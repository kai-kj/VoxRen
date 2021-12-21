#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "interface.h"

typedef struct ComponentsIDs {
	ComponentID red;
	ComponentID green;
	ComponentID blue;
	ComponentID material;
	ComponentID v1Tag;
	ComponentID v2Tag;
	ComponentID v3Tag;
	ComponentID v1;
	ComponentID v2;
	ComponentID v3;

	ComponentID fps;
	ComponentID rps;
	ComponentID lookingAtPixel;
	ComponentID lookingAtVoxel;
	ComponentID lookingAtNormal;
	ComponentID lookingAtDistance;
	ComponentID voxCount;
	ComponentID chunkCount;

	ComponentID cameraX;
	ComponentID cameraY;
	ComponentID cameraZ;
	ComponentID cameraPitch;
	ComponentID cameraYaw;
	ComponentID cameraSensorWidth;
	ComponentID cameraFocalLength;
	ComponentID cameraAperture;
	ComponentID cameraExposure;
	ComponentID lookSensitivity;
	ComponentID moveSensitivity;
	ComponentID bgRed;
	ComponentID bgGreen;
	ComponentID bgBlue;
	ComponentID bgBrightness;

	ComponentID chunkSize;
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