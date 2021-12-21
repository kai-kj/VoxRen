// built-in opencl constants don't seem to work
#define PI 3.14159265359f
#define BIG_NUM 2147483647.0f
#define SMALL_NUM 0.00001f

#define MATERIAL_TYPE_EMPTY 0
#define MATERIAL_TYPE_LIGHT_SOURCE 1
#define MATERIAL_TYPE_LAMBERTIAN 2
#define MATERIAL_TYPE_METAL 3
#define MATERIAL_TYPE_DIELECTRIC 4

//--------------------------------------------------------------------------------------------------------------------//
// structs                                                                                                            //
//--------------------------------------------------------------------------------------------------------------------//

typedef struct Ray {
	float3 origin;
	float3 direction;
} Ray;

typedef struct Material {
	int type;
	float3 color;

	float v1;
	float v2;
	float v3;
} Material;

typedef struct Camera {
	float3 pos;
	float2 rot;
	float sensorWidth;
	float focalLength;
	float aperture;
	float exposure;
} Camera;

typedef struct Chunk {
	int3 pos;
	int firstVoxel;
	int voxelCount;
} Chunk;

typedef struct Voxel {
	int3 pos;
	Material material;
} Voxel;

typedef struct Renderer {
	int2 imageSize;
	global float3 *image;

	int voxelCount;
	global Voxel *voxels;

	int chunkSize;
	int chunkCount;
	global Chunk *chunks;

	float3 bgColor;
	float bgBrightness;

	Camera camera;
	ulong *rng;
} Renderer;

//--------------------------------------------------------------------------------------------------------------------//
// rng                                                                                                                //
//--------------------------------------------------------------------------------------------------------------------//

ulong init_rng_1(ulong a) {
	return (16807 * a) % 2147483647 * (16807 * a) % 2147483647;
}

ulong init_rng_2(ulong a, ulong b) {
	return (16807 * a * b) % 2147483647 * (16807 * a * b) % 2147483647;
}

float get_rand_fraction(ulong *rng) {
	*rng = (16807 * *rng) % 2147483647;
	return *rng / 2147483647.0f;
}

float get_rand_float_in_range(ulong *rng, float a, float b) {
	return a + get_rand_fraction(rng) * (b - a);
}

float3 get_random_unit_vector(ulong *rng) {
	float theta = get_rand_float_in_range(rng, 0, 2 * PI);
	float z = get_rand_float_in_range(rng, -1, 1);
	float sqrtZ = sqrt(1 - z * z);

	return (float3){sqrtZ * cos(theta), sqrtZ * sin(theta), z};
}

#define get_max_idx(t, a, b) (t[a] > t[b] ? a : b)
#define get_min_idx(t, a, b) (t[a] < t[b] ? a : b)

//--------------------------------------------------------------------------------------------------------------------//
// math                                                                                                               //
//--------------------------------------------------------------------------------------------------------------------//

float3 rotate_vector(float3 vec, float3 rot) {
	float3 nVec;

	nVec.x = vec.x * cos(rot.z) - vec.y * sin(rot.z);
	nVec.y = vec.x * sin(rot.z) + vec.y * cos(rot.z);

	nVec.x = vec.x * cos(rot.y) + vec.z * sin(rot.y);
	nVec.z = -vec.x * sin(rot.y) + vec.z * cos(rot.y);

	nVec.y = vec.y * cos(rot.x) - vec.z * sin(rot.x);
	nVec.z = vec.y * sin(rot.x) + vec.z * cos(rot.x);

	return nVec;
}

float3 get_reflection_dir(float3 in, float3 normal) {
	return in - normal * 2 * dot(in, normal);
}

float3 get_refraction_dir(float3 in, float3 normal, float relativeRefIdx) {
	float cosTheta = fmin(dot(in * -1, normal), 1);
	float3 outPerpendicular = (in + normal * cosTheta) * relativeRefIdx;
	float3 outParallel = normal * -sqrt(fabs(1 - length(outPerpendicular) * length(outPerpendicular)));
	return outParallel + outPerpendicular;
}

float get_reflectance(float cosTheta, float relativeRefIdx) {
	float r = (1 - relativeRefIdx) / (1 + relativeRefIdx);
	r = r * r;
	return r + (1 - r) * pow((1 - cosTheta), 5);
}

bool ray_voxel(Ray ray, global Voxel *voxel, float3 dirFrac, float *tMin) {
	float t[6];
	t[0] = (voxel->pos.x - ray.origin.x) * dirFrac.x;
	t[1] = t[0] + dirFrac.x;
	t[2] = (voxel->pos.y - ray.origin.y) * dirFrac.y;
	t[3] = t[2] + dirFrac.y;
	t[4] = (voxel->pos.z - ray.origin.z) * dirFrac.z;
	t[5] = t[4] + dirFrac.z;

	*tMin = max(max(min(t[0], t[1]), min(t[2], t[3])), min(t[4], t[5]));
	float tMax = min(min(max(t[0], t[1]), max(t[2], t[3])), max(t[4], t[5]));

	return tMax > *tMin && tMax >= 0;
}

// constant int3 returnValues[6] = {(int3){-1, 0, 0}, (int3){1, 0, 0},	 (int3){0, -1, 0},
// 								 (int3){0, 1, 0},  (int3){0, 0, -1}, (int3){0, 0, 1}};

int3 get_ray_voxel_normal(Ray ray, global Voxel *voxel, float3 dirFrac) {
	int3 returnValues[6] = {(int3){-1, 0, 0}, (int3){1, 0, 0},	(int3){0, -1, 0},
							(int3){0, 1, 0},  (int3){0, 0, -1}, (int3){0, 0, 1}};

	float t[6];

	t[0] = (voxel->pos.x - ray.origin.x) * dirFrac.x;
	t[1] = t[0] + dirFrac.x;
	t[2] = (voxel->pos.y - ray.origin.y) * dirFrac.y;
	t[3] = t[2] + dirFrac.y;
	t[4] = (voxel->pos.z - ray.origin.z) * dirFrac.z;
	t[5] = t[4] + dirFrac.z;

	int tMinIdx = get_max_idx(t, get_max_idx(t, get_min_idx(t, 0, 1), get_min_idx(t, 2, 3)), get_min_idx(t, 4, 5));

	return returnValues[tMinIdx];
}

bool ray_chunk(Ray ray, global Chunk *chunk, int chunkSize, float3 dirFrac) {
	float t[6];
	t[0] = (chunk->pos.x * chunkSize - ray.origin.x) * dirFrac.x;
	t[1] = t[0] + dirFrac.x * chunkSize;
	t[2] = (chunk->pos.y * chunkSize - ray.origin.y) * dirFrac.y;
	t[3] = t[2] + dirFrac.y * chunkSize;
	t[4] = (chunk->pos.z * chunkSize - ray.origin.z) * dirFrac.z;
	t[5] = t[4] + dirFrac.z * chunkSize;

	float tMin = max(max(min(t[0], t[1]), min(t[2], t[3])), min(t[4], t[5]));
	float tMax = min(min(max(t[0], t[1]), max(t[2], t[3])), max(t[4], t[5]));

	return tMax > tMin && tMax >= 0;
}

//--------------------------------------------------------------------------------------------------------------------//
// ray                                                                                                                //
//--------------------------------------------------------------------------------------------------------------------//

bool cast_ray(Renderer *r, Ray ray, float3 *hitPos, int3 *normal, Voxel *voxel) {
	bool hit = false;
	float minDist;
	int minIdx;

	float3 dirFrac = (float3){(ray.direction.x != 0) ? (1.0f / ray.direction.x) : BIG_NUM,
							  (ray.direction.y != 0) ? (1.0f / ray.direction.y) : BIG_NUM,
							  (ray.direction.z != 0) ? (1.0f / ray.direction.z) : BIG_NUM};

	for (uint i = 0; i < r->chunkCount; i++) {
		global Chunk *chunk = &r->chunks[i];

		if (ray_chunk(ray, chunk, r->chunkSize, dirFrac)) {
			int lastVoxel = chunk->firstVoxel + chunk->voxelCount;

			for (uint j = chunk->firstVoxel; j < lastVoxel; j++) {
				float t;
				if (ray_voxel(ray, &r->voxels[j], dirFrac, &t)) {
					if (!hit || t < minDist) {
						hit = true;
						minDist = t;
						minIdx = j;
					}
				}
			}
		}
	}

	if (!hit) return false;

	*hitPos = ray.origin + ray.direction * minDist;
	*normal = get_ray_voxel_normal(ray, &r->voxels[minIdx], dirFrac);
	*voxel = r->voxels[minIdx];

	return true;
}

float3 get_color(Renderer *r, Ray ray, int maxDepth) {
	// TODO: maxDepth
	// TODO: russian rulette

	int returnFlag = false;
	float3 mask = 1;
	float3 color = 0;

	for (int i = 0; i < 10; i++) {
		float3 hitPos;
		int3 iNormal;
		Voxel voxel;

		if (cast_ray(r, ray, &hitPos, &iNormal, &voxel)) {
			Material material = voxel.material;
			float3 fNormal = convert_float3(iNormal);
			hitPos += fNormal * SMALL_NUM;

			// TODO: dielectric material
			switch (material.type) {
				case MATERIAL_TYPE_LIGHT_SOURCE:
					// TODO: better lighting
					color = i == 0 ? material.color : material.color * mask * material.v1;
					// color = (mask + material.color * material.v1) / (material.v1 + 1);
					returnFlag = true;
					break;

				case MATERIAL_TYPE_LAMBERTIAN:
					ray = (Ray){hitPos, normalize(fNormal + get_random_unit_vector(r->rng))};
					mask *= material.color;
					break;

				case MATERIAL_TYPE_METAL:
					ray = (Ray){hitPos,
								normalize(get_reflection_dir(ray.direction, fNormal) +
										  get_random_unit_vector(r->rng) * material.v1)};
					mask = mask * (1 - material.v2) + mask * material.color * material.v2;
					break;

				case MATERIAL_TYPE_DIELECTRIC:
					break;

				default:
					break;
			}

		} else {
			color = mask * r->bgColor * r->bgBrightness;
			returnFlag = true;
		}

		if (returnFlag) break;
	}

	return color;
}

//--------------------------------------------------------------------------------------------------------------------//
// main                                                                                                               //
//--------------------------------------------------------------------------------------------------------------------//

// TODO: better sampling

Ray get_first_ray(Renderer *r, int id, bool blur) {
	float aspectRatio = (float)r->imageSize.x / (float)r->imageSize.y;

	int x = id % r->imageSize.x;
	int y = id / r->imageSize.x;

	float3 offset = (float3){r->camera.sensorWidth * (x - r->imageSize.x / 2) / r->imageSize.x,
							 r->camera.sensorWidth * (y - r->imageSize.y / 2) / aspectRatio / r->imageSize.y,
							 r->camera.focalLength};

	float3 origin = r->camera.pos + offset;
	float3 target = blur ? r->camera.pos + get_random_unit_vector(r->rng) * r->camera.aperture : r->camera.pos;

	float3 direction = -normalize(target - origin);

	float cosX = cos(r->camera.rot.x);
	float sinX = sin(r->camera.rot.x);
	float cosY = cos(r->camera.rot.y);
	float sinY = sin(r->camera.rot.y);

	direction = (float3){direction.x, direction.y * cosY - direction.z * sinY, direction.y * sinY + direction.z * cosY};

	direction = (float3){
		direction.x * cosX - direction.z * sinX,
		direction.y,
		direction.x * sinX + direction.z * cosX,
	};

	return (Ray){r->camera.pos, direction};
}

float3 adjust_color(Renderer *r, float3 color) {
	return color * r->camera.exposure * r->camera.aperture;
}

kernel void pathtracer(int2 imageSize, global float3 *image, int voxelCount, global Voxel *voxels, int chunkSize,
					   int chunkCount, global Chunk *chunks, float3 bgColor, float bgBrightness, Camera camera,
					   global int3 *lookingAt, int sampleNumber, ulong seed) {

	int id = get_global_id(0);
	ulong rng = init_rng_2(id, seed);

	Renderer r = (Renderer){imageSize, image,	voxelCount,	  voxels, chunkSize, chunkCount,
							chunks,	   bgColor, bgBrightness, camera, &rng};

	float3 color = get_color(&r, get_first_ray(&r, id, true), 10);

	color = adjust_color(&r, color);

	if (id % r.imageSize.x == lookingAt[2].x && id / r.imageSize.x == lookingAt[2].y) {
		float3 hitPos;
		int3 normal;
		Voxel voxel;

		if (cast_ray(&r, get_first_ray(&r, id, false), &hitPos, &normal, &voxel)) {
			lookingAt[0] = voxel.pos;
			lookingAt[1] = normal;
			lookingAt[2].z = (int)(distance(camera.pos, hitPos) * 100);
		} else {
			lookingAt[2].z = -1;
		}
	}

	image[id] = (image[id] * sampleNumber + color) / (sampleNumber + 1);
}