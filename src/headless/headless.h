#ifndef HEADLESS_H
#define HEADLESS_H

#include <stb/stb_image_write.h>

#include "../renderer/renderer.h"
#include "status.h"

Status render_image(int samples, char *fileName);

#endif