//
// Created by zyuiop on 20/05/19.
//

#include "World_Map.h"

World_Map::World_Map(std::vector<Voxel> voxels) {
	_cubes = std::vector<Cube*>();


	for (auto voxel : voxels) {
		if (voxel.z < 19)
			continue; // ignore the base

		min_x = std::min(min_x, (int) voxel.x);
		min_y = std::min(min_y, (int) voxel.y);
		min_z = std::min(min_z, (int) voxel.z);
		max_x = std::max(max_x, (int) voxel.x);
		max_y = std::max(max_y, (int) voxel.y);
		max_z = std::max(max_z, (int) voxel.z);

		// Reverse y and z
		_cubes.push_back(new Cube(voxel.x, voxel.z, voxel.y, voxel.get_color(), this));
	}

	_cubes_positions = std::vector<std::vector<std::vector<bool>>>(max_x - min_x + 1, std::vector<std::vector<bool>>(max_y - min_y + 1, std::vector<bool>(max_z - min_z + 1)));

	for (auto voxel : voxels) {

		if (voxel.z < 19)
			continue; // ignore the base

		_cubes_positions[voxel.x - min_x][voxel.y - min_y][voxel.z - min_z] = true;
	}

	std::cout << "Lower " << min_x << " " << min_y << " " << min_z << std::endl;
	std::cout << "Upper " << max_x << " " << max_y << " " << max_z << std::endl;
	std::cout << "Size " << _cubes.size();
}

bool World_Map::has_block(int x, int y, int z) {
	if (x - min_x < _cubes_positions.size()) {
		auto x_list = _cubes_positions[x - min_x];

		if (y - min_y < x_list.size()) {
			auto y_list = x_list[y - min_y];

			if (z - min_z < y_list.size()) {
				return y_list[z - min_z];
			}
		}
	}

	return false;
}

World_Map::~World_Map() {
	for (Cube* v : _cubes) {
		delete v;
	}
}
