//
// Created by zyuiop on 20/05/19.
//

#ifndef RANDOMTILEMAPGENERATION_WORLD_MAP_H
#define RANDOMTILEMAPGENERATION_WORLD_MAP_H


#include <vector>
#include <voxels.h>
#include "cube.h"

class World_Map {
public:
	World_Map(std::vector<Voxel> voxels);

	~World_Map();

	std::vector<Cube*> _cubes;

	bool has_block(int x, int y, int z);

	vec4 start_position();
	void draw();

private:
	std::vector<std::vector<std::vector<bool>>> _cubes_positions;

	int min_x = (1 << 24), min_y = (1 << 24), min_z = (1 << 24), max_x = (0), max_y = (0), max_z = (0);

	int triangles = 0;

	/// vertex array object
	GLuint vao_ = 0;
	/// vertex buffer object
	GLuint vbo_ = 0;
	/// normals buffer object
	GLuint nbo_ = 0;
	/// colors buffer object
	GLuint cbo_ = 0;
	/// index buffer object
	GLuint ibo_ = 0;

	void initialize();
};


#endif //RANDOMTILEMAPGENERATION_WORLD_MAP_H
