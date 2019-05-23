//
// Created by zyuiop on 20/05/19.
//

#include "World_Map.h"

World_Map::World_Map(std::vector<Voxel> voxels) {
	_cubes = std::vector<Cube*>();


	for (auto voxel : voxels) {
		if (voxel.x == 80 && voxel.y == 80) {
			std::cout << (int) voxel.z << " " << (int) voxel.color << " " << voxel.get_color() << std::endl;
		}

		if (voxel.z != 19)
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

		if (voxel.z != 19)
			continue; // ignore the base

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
	glDrawElements(GL_TRIANGLES, triangles, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void World_Map::initialize() {
	std::vector<GLuint> indices;
	std::vector<float> norm;
	std::vector<float> vertices;

	int i = 0;
	for (Cube* c : _cubes) {
		c->add_to_chunk(i ++, vertices, indices, norm);
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

	// triangle indices
	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}