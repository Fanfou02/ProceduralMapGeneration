//
// Created by zyuiop on 20/05/19.
//

#include "World_Map.h"

World_Map::World_Map(std::vector<Voxel> voxels) {
	_cubes = std::vector<Cube*>();


	for (auto voxel : voxels) {
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
		_cubes_positions[voxel.x - min_x][voxel.y - min_y][voxel.z - min_z] = true;
	}

	std::cout << "Lower " << min_x << " " << min_y << " " << min_z << std::endl;
	std::cout << "Upper " << max_x << " " << max_y << " " << max_z << std::endl;
	std::cout << "Size " << _cubes.size() << std::endl;
}



bool World_Map::has_block(int x, int y, int z) {
	return false;

	if (x - min_x < _cubes_positions.size()) {
		auto x_list = _cubes_positions[x - min_x];

		// Reverse y and z
		if (z - min_y < x_list.size()) {
			auto y_list = x_list[z - min_y];

			if (y - min_z < y_list.size()) {
				return y_list[y - min_z];
			}
		}
	}

	return false;
}

vec4 World_Map::start_position() {
	return vec4(max_x + 5, max_z + 5, max_y + 5, 0);
}

World_Map::~World_Map() {
	for (Cube* v : _cubes) {
		delete v;
	}
}

void World_Map::draw() {
	if (vao_ == 0) initialize();
	if (triangles == 0) return;

	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, triangles);

	glBindVertexArray(0);
}

void World_Map::initialize() {
	std::vector<float> norm;
	std::vector<float> vertices;
	std::vector<float> colors;

	for (Cube* c : _cubes) {
		c->add_to_chunk(vertices, norm, colors);
		triangles += 36;
	}

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);


	// vertex positions -> attribute 0
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Normal vectors -> attribute 1
	glGenBuffers(1, &nbo_);
	glBindBuffer(GL_ARRAY_BUFFER, nbo_);
	glBufferData(GL_ARRAY_BUFFER, norm.size()*sizeof(float), &norm[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);

	// Colors buffer -> attribute 2
	glGenBuffers(1, &cbo_);
	glBindBuffer(GL_ARRAY_BUFFER, cbo_);
	glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}