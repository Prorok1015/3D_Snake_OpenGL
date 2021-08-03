#pragma once

#ifndef CHUNK_H_
#define CHUNK_H_

#define CHUNK_W		16
#define CHUNK_H	16
#define CHUNK_D		16
#define CHUNK_VOL	(CHUNK_W * CHUNK_H * CHUNK_D)

class voxel;

class Chunk
{
public:
	int x, y, z;
	voxel* voxels;
	bool modified = true;
	Chunk(int x, int y, int z);
	~Chunk();
};

#endif // !Chunk_H