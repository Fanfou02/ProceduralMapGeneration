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

private:
	std::vector<std::vector<std::vector<bool>>> _cubes_positions;

	int min_x = (1 << 24), min_y = (1 << 24), min_z = (1 << 24), max_x = (0), max_y = (0), max_z = (0);
};


#endif //RANDOMTILEMAPGENERATION_WORLD_MAP_H
