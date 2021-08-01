#include "VoxelRenderer.h"
#include "Mesh.h"
#include "Chank.h"
#include "voxel.h"


#define VERTEX_SIZE (3 + 2 + 1)

#define IS_IN(X,Y,Z) ((X) >= 0 && (X) < CHANK_WIDTH && (Y) >= 0 && (Y) < CHANK_HEIGHT && (Z) >= 0 && (Z) < CHANK_DEPHT)
#define VOXEL(X,Y,Z) (chunk->voxels[((Y) * CHANK_DEPHT + (Z)) * CHANK_WIDTH + (X)])
#define IS_BLOCKED(X,Y,Z) ((IS_IN(X, Y, Z)) && VOXEL(X, Y, Z).id)

#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = (U);\
								  buffer[INDEX+4] = (V);\
								  buffer[INDEX+5] = (L);\
								  INDEX += VERTEX_SIZE;

int chunk_attrs[] = { 3,2,1, 0 };

VoxelRenderer::VoxelRenderer(size_t capacity): capacity(capacity)
{
	buffer = new float[capacity * VERTEX_SIZE * 6];
}

VoxelRenderer::~VoxelRenderer()
{
	delete[] buffer;
}

Mesh* VoxelRenderer::render(Chank* chunk) {
	size_t index = 0;
	for (int y = 0; y < CHANK_HEIGHT; y++) {
		for (int z = 0; z < CHANK_DEPHT; z++) {
			for (int x = 0; x < CHANK_WIDTH; x++) {
				voxel vox = chunk->voxels[(y * CHANK_DEPHT + z) * CHANK_WIDTH + x];
				unsigned int id = vox.id;

				if (!id) {
					continue;
				}

				float l;
				float uvsize = 1.0f / 16.0f;
				float u = (id % 16) * uvsize;
				float v = 1.f - ((float)(1 + (id / 16u)) * uvsize);

				if (!IS_BLOCKED(x, y + 1, z)) {
					l = 1.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
				}
				if (!IS_BLOCKED(x, y - 1, z)) {
					l = 0.75f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}

				if (!IS_BLOCKED(x + 1, y, z)) {
					l = 0.95f;
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
				}
				if (!IS_BLOCKED(x - 1, y, z)) {
					l = 0.85f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}

				if (!IS_BLOCKED(x, y, z + 1)) {
					l = 0.9f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
				}
				if (!IS_BLOCKED(x, y, z - 1)) {
					l = 0.8f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
				}
			}
		}
	}
	return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}