#include "headless.h"

#define GC_VALUE(v) sqrt(v)
#define FLT_TO_CHAR(f) (char)((f <= 0) ? 0 : (f >= 1) ? 255 : (int)(f * 255))

static char *_get_stb_image() {
	char *pixels = malloc(ren.image.size.x * ren.image.size.y * 3);

	for (int i = 0; i < ren.image.size.x * ren.image.size.y; i++) {
		pixels[i * 3 + 0] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].x));
		pixels[i * 3 + 1] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].y));
		pixels[i * 3 + 2] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].z));
	}

	return pixels;
}

static char *_get_file_ext(const char *filename) {
	char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

static void _write_image(char *image, char *fileName) {
	char *format = _get_file_ext(fileName);

	if (strcmp(format, "png") == 0)
		stbi_write_png(fileName, ren.image.size.x, ren.image.size.y, 3, image, 0);

	else if (strcmp(format, "bmp") == 0)
		stbi_write_bmp(fileName, ren.image.size.x, ren.image.size.y, 3, image);

	else if (strcmp(format, "jpg") == 0 || strcmp(format, "jpeg") == 0)
		stbi_write_jpg(fileName, ren.image.size.x, ren.image.size.y, 3, image, 95);
}

Status render_image(int samples, char *fileName) {
	msg("Rendering %d samples to %s", samples, fileName);

	setup_renderer_args();

	for (int i = 0; i < samples; i++) {
		render_frame(i);

		printf("\r      > Rendering frame [%d/%d] (%02.2f%%)", i, samples, (float)i / (float)samples * 100.0);
		fflush(stdout);
	}

	printf("\n");

	char *image = _get_stb_image();
	_write_image(image, fileName);
	free(image);

	destroy_renderer();

	return SUCCESS;
}