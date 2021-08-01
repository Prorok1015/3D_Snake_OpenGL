#pragma once

#ifndef CHANK_H_
#define CHANK_H_

#define CHANK_WIDTH		16
#define CHANK_HEIGHT	16
#define CHANK_DEPHT		16
#define CHANK_VOL	(CHANK_WIDTH * CHANK_HEIGHT * CHANK_DEPHT)

class voxel;

class Chank
{

public:
	voxel* voxels;

	Chank();
	~Chank();
};

#endif // !CHANK_H