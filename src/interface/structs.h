#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "interface.h"

typedef struct ComponentsIDs {
	ComponentID red;
	ComponentID green;
	ComponentID blue;
	ComponentID material;
	ComponentID v1;
	ComponentID v2;
	ComponentID v3;

	ComponentID fps;
	ComponentID rps;
	ComponentID lookingAtPixel;
	ComponentID lookingAtVoxel;
	ComponentID lookingAtNormal;
} ComponentIDs;

typedef struct GUIState {
	Texture2D renderTexture;

	int renderMousePosX;
	int renderMousePosY;

	int removeVoxel;
	VoxMaterial selectedMaterial;
	cl_float3 selectedBgColor;

	float cameraMoveSpeed;
	float cameraLookSpeed;

	ComponentIDs components;

} GUIState;

#endif