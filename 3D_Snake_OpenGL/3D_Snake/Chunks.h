#pragma once
#include <glm/glm.hpp>

class Chunk;
class voxel;

class Chunks
{
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w, h, d;

	Chunks(int w, int h, int d);
	~Chunks();

	Chunk* getChunk(int x, int y, int z);
	voxel* get(int x, int y, int z);
	void set(int x, int y, int z, int id);
	voxel* rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

	void write(unsigned char* dest);
	void read(unsigned char* source);
};

