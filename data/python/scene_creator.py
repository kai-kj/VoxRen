import struct

class Material:
	def __init__(self, materialType: int, color: (float, float, float), properties: (float, float, float)):
		self.color = color
		self.materialType = materialType
		self.properties = properties

class Voxel:
	def __init__(self, pos: (int, int, int), material: Material):
		self.pos = pos
		self.material = material

class Camera:
	def __init__(self):
		self.pos = (0, 0, 0)
		self.rot = (0, 0)
		self.properties = (10, 10, 0.0001, 10000)
	
class Scene:
	def __init__(self):
		self.voxels = []
		self.camera = Camera()
		self.bgColor = (1.0, 1.0, 1.0)
		self.bgBrightness = 0.5

	def add_voxel(self, pos: (int, int, int), material: Material):
		self.voxels.append(Voxel(pos, material))
	
	def moveCamera(self, pos: (float, float, float)):
		self.camera.pos = pos
	
	def rotateCamera(self, rot: (float, float)):
		self.camera.rot = rot
	
	def setCameraProperties(self, sensorWidth: float, focalLength: float, aperture: float, exposure: float):
		self.camera.properties = (sensorWidth, focalLength, aperture, exposure)
	
	def setBackground(self, color: (float, float, float), brightness: float):
		self.bgColor = color
		self.bgBrightness = brightness
	
	def read(self, fileName: str):
		f = open(fileName, "rb")
		self.camera.pos = struct.unpack("f" * 3, f.read(4 * 3))
		self.camera.rot = struct.unpack("f" * 2, f.read(4 * 2))
		self.camera.properties = struct.unpack("f" * 4, f.read(4 * 4))
		self.bgColor = struct.unpack("f" * 3, f.read(4 * 3))
		self.bgBrightness = struct.unpack("f", f.read(4))[0]
		voxelCount = struct.unpack("i", f.read(4))[0]

		for i in range(voxelCount):
			pos = struct.unpack("i" * 3, f.read(4 * 3))
			materialType = struct.unpack("i", f.read(4))[0]
			color = struct.unpack("f" * 3, f.read(4 * 3))
			properties = struct.unpack("f" * 3, f.read(4 * 3))
			self.add(Voxel(pos, Material(materialType, color, properties)))
		
		f.close()

	def write(self, fileName: str):
		f = open(fileName, "wb")
		f.write(struct.pack("f" * 3, self.camera.pos[0], self.camera.pos[1], self.camera.pos[2]))
		f.write(struct.pack("f" * 2, self.camera.rot[0], self.camera.rot[1]))
		f.write(struct.pack("f" * 4, self.camera.properties[0], self.camera.properties[1], self.camera.properties[2], self.camera.properties[3]))
		f.write(struct.pack("f" * 3, self.bgColor[0], self.bgColor[1], self.bgColor[2]))
		f.write(struct.pack("f", self.bgBrightness))
		f.write(struct.pack("i", len(self.voxels)))

		for voxel in self.voxels:
			f.write(struct.pack("i" * 3, voxel.pos[0], voxel.pos[1], voxel.pos[2]))
			f.write(struct.pack("i", voxel.material.materialType))
			f.write(struct.pack("f" * 3, voxel.material.color[0], voxel.material.color[1], voxel.material.color[2]))
			f.write(struct.pack("f" * 3, voxel.material.properties[0], voxel.material.properties[1], voxel.material.properties[2]))
		
		f.close()

class LightSourceMaterial(Material):
	def __init__(self, color: (float, float, float), brightness: float):
		super().__init__(1, color, (brightness, 0, 0))

class LambertMaterial(Material):
	def __init__(self, color: (float, float, float)):
		super().__init__(2, color, (0, 0, 0))

class MetalMaterial(Material):
	def __init__(self, color: (float, float, float), tint: float, fuzz: float):
		super().__init__(3, color, (tint, fuzz, 0))

class GlassMaterial(Material):
	def __init__(self, color: (float, float, float), tint: float, fuzz: float, refIdx: float):
		super().__init__(4, color, (tint, fuzz, refIdx))