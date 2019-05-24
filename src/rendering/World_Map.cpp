//
// Created by zyuiop on 20/05/19.
//

#include "World_Map.h"
#define CUBE_EVERY 0.05
#define STEP_BY_STEP_ENABLE true
#define STEP_BY_STEP_FIXED_LAYER 19

World_Map::World_Map(std::vector<Voxel> voxels) {
	_cubes = std::queue<Cube*>();
	_spawned_cubes = std::vector<Cube*>();


	for (auto voxel : voxels) {
		if (voxel.z < 17) continue;

		min_x = std::min(min_x, (int) voxel.x);
		min_y = std::min(min_y, (int) voxel.y);
		min_z = std::min(min_z, (int) voxel.z);
		max_x = std::max(max_x, (int) voxel.x);
		max_y = std::max(max_y, (int) voxel.y);
		max_z = std::max(max_z, (int) voxel.z);

		// Reverse y and z
		Cube* cube = new Cube(voxel.x, voxel.z, voxel.y, voxel.get_color(), this);

		if (STEP_BY_STEP_ENABLE && cube->target_y > STEP_BY_STEP_FIXED_LAYER)
			_cubes.push(cube);
		else
			_spawned_cubes.push_back(cube);
	}

	std::cout << "Lower " << min_x << " " << min_y << " " << min_z << std::endl;
	std::cout << "Upper " << max_x << " " << max_y << " " << max_z << std::endl;
	std::cout << "Size " << _cubes.size() << std::endl;
}

vec4 World_Map::start_position() {
	return vec4(max_x + 5, max_z + 5, max_y + 5, 0);
}

World_Map::~World_Map() {
	while (_cubes.front() != NULL) {
		delete _cubes.front();
		_cubes.pop();
	}
}

void World_Map::draw() {
	if (vao_ == 0) initialize();
	if (triangles == 0) return;

	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, triangles);

	glBindVertexArray(0);
}

void World_Map::update_position(Cube* cube) {
	std::vector<float> vertices;
	cube->add_position_to_chunk(vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferSubData(GL_ARRAY_BUFFER, cube->offset * 3 * sizeof(float), vertices.size() * sizeof(float), vertices.data());
}

void World_Map::timer(float seconds) {
	next_block -= seconds;

	std::vector<int> to_erase;

	int i = 0;
	for (Cube* cube : _spawned_cubes) {
		bool keep = cube->timer(seconds);
		if (!keep) {
			to_erase.push_back(i);
		}

		i++;
	}

	for (i = 0; i < to_erase.size(); ++i) {
		_spawned_cubes.erase(_spawned_cubes.begin() + to_erase[i] - i);
	}

	while (next_block <= 0) {
		auto cube = _cubes.front();
		add_cube(cube);
		_cubes.pop();

		cube->timer(-next_block);

		next_block += CUBE_EVERY;
	}

}

void World_Map::add_cube(Cube* cube, bool reset_position) {
	if (cube->offset != -1) {
		update_position(cube);
		return;
	}

	std::vector<float> norm;
	std::vector<float> vertices;
	std::vector<float> colors;

	if (reset_position) {
		cube->y = cube->target_y;
	} else {
		_spawned_cubes.push_back(cube);
	}

	cube->add_to_chunk(vertices, norm, colors);
	cube->offset = triangles;
	triangles += 36;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferSubData(GL_ARRAY_BUFFER, cube->offset* 3 * sizeof(float), vertices.size() * sizeof(float), vertices.data());


	glBindBuffer(GL_ARRAY_BUFFER, nbo_);
	glBufferSubData(GL_ARRAY_BUFFER, cube->offset* 3 * sizeof(float), norm.size() * sizeof(float), norm.data());


	glBindBuffer(GL_ARRAY_BUFFER, cbo_);
	glBufferSubData(GL_ARRAY_BUFFER, cube->offset* 3 * sizeof(float), colors.size() * sizeof(float), colors.data());
}

void World_Map::initialize() {
	size_t size = _cubes.size() * 36 * 3 * sizeof(float);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);


	// vertex positions -> attribute 0
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Normal vectors -> attribute 1
	glGenBuffers(1, &nbo_);
	glBindBuffer(GL_ARRAY_BUFFER, nbo_);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);

	// Colors buffer -> attribute 2
	glGenBuffers(1, &cbo_);
	glBindBuffer(GL_ARRAY_BUFFER, cbo_);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (auto cube : _spawned_cubes) {
		add_cube(cube, true);
	}

	_spawned_cubes.clear();
}