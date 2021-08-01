#include "Chank.h"
#include "voxel.h"
#include <cmath>

Chank::Chank()
{
	voxels = new voxel[CHANK_VOL];
	for (int y = 0; y < CHANK_HEIGHT; y++) {
		for (int z = 0; z < CHANK_DEPHT; z++) {
			for (int x = 0; x < CHANK_WIDTH; x++) {
				int id = y <= 15;//(sin(x * 0.3f) * 0.5f + 0.5f) * 10;
				if (y <= 2)
					id = 2;
				voxels[(y * CHANK_DEPHT + z) * CHANK_WIDTH + x].id = id;
			}
		}
	}
}

Chank::~Chank()
{
	delete[] voxels;
}