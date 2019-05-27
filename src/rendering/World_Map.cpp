/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#include "World_Map.h"
#define CUBE_EVERY 1
#define SPAWN_GROUPED true
#define STEP_BY_STEP_ENABLE true

World_Map::World_Map(std::vector<Voxel> voxels, int groundLevel, int groupSize) {
	_not_spawned_cubes = std::queue<Cube*>();
	_moving_cubes = std::vector<Cube*>();
	_floor = std::vector<Cube*>();
	_rest = std::vector<Cube*>();
	_group_size = groupSize;

	for (auto voxel : voxels) {
		min_x = std::min(min_x, (int) voxel.x);
		min_y = std::min(min_y, (int) voxel.y);
		min_z = std::min(min_z, (int) voxel.z);
		max_x = std::max(max_x, (int) voxel.x);
		max_y = std::max(max_y, (int) voxel.y);
		max_z = std::max(max_z, (int) voxel.z);

		// Reverse y and z
		Cube* cube = new Cube(voxel.x, voxel.z, voxel.y, voxel.get_color(), this);

		if (STEP_BY_STEP_ENABLE && cube->target_y > groundLevel) {
			_rest.push_back(cube);
		} else {
			_floor.push_back(cube);
		}
	}
}

vec4 World_Map::start_position() {
	return vec4(max_x + 5, max_z + 5, max_y + 5, 0);
}

World_Map::~World_Map() {
	while (!_not_spawned_cubes.empty()) {
		delete _not_spawned_cubes.front();
		_not_spawned_cubes.pop();
	}

	for (auto cube : _moving_cubes) {
		delete cube;
	}
}

void World_Map::draw() {
	if (vao_ == 0) initialize();
	if (rendered_triangles == 0) return;

	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, rendered_triangles);
	glBindVertexArray(0);
}

void World_Map::update_position(Cube* cube) {
	std::vector<float> vertices;
	cube->add_position_to_chunk(vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferSubData(GL_ARRAY_BUFFER, cube->offset, vertices.size() * sizeof(float), vertices.data());
}

void World_Map::timer(float seconds) {
	if (vao_ == 0) return;

	next_block -= seconds;
	std::vector<int> to_erase;

	int i = 0;
	for (Cube* cube : _moving_cubes) {
		bool keep = cube->timer(seconds);
		if (!keep) {
			to_erase.push_back(i);
		}

		i++;
	}

	for (i = 0; i < to_erase.size(); ++i) {
		_moving_cubes.erase(_moving_cubes.begin() + to_erase[i] - i);
	}

	while (!_not_spawned_cubes.empty() && next_block <= 0) {
		auto cube = _not_spawned_cubes.front();

		if (SPAWN_GROUPED) {
			int min_x = (cube->x / _group_size) * _group_size;
			int min_y = (cube->target_y / _group_size) * _group_size;
			int min_z = (cube->z / _group_size) * _group_size;

			int max_x = min_x + _group_size;
			int max_y = min_y + _group_size;
			int max_z = min_z + _group_size;

			while (!_not_spawned_cubes.empty() && (cube = _not_spawned_cubes.front()) && (
					cube->x >= min_x && cube->x < max_x
					&& cube->target_y >= min_y && cube->target_y < max_y
					&& cube->z >= min_z && cube->z < max_z)) {
				_moving_cubes.push_back(cube);
				_not_spawned_cubes.pop();

				rendered_triangles += 36;
			}
		} else {
			_moving_cubes.push_back(cube);
			_not_spawned_cubes.pop();

			rendered_triangles += 36;
		}

		cube->timer(-next_block);

		next_block += CUBE_EVERY;
	}


}

void World_Map::initialize() {
	std::vector<float> norm;
	std::vector<float> vertices;
	std::vector<float> colors;

	const size_t offset_step = 36 * 3 * sizeof(float);

	// Add the floor first, at the begining of the array
	for (auto cube : _floor) {
		cube->y = cube->target_y;
		cube->offset = offset;
		offset += offset_step;

		cube->add_to_chunk(vertices, norm, colors);

		// Render them from the start
		rendered_triangles += 36;
	}

	// Add the other cubes in the array, but don't increase rendered_triangles (effectively not rendering them)
	for (auto cube : _rest) {
		cube->offset = offset;
		offset += offset_step;

		cube->add_to_chunk(vertices, norm, colors);

		// Add them to the not spawned queue
		_not_spawned_cubes.push(cube);
	}

	_floor.clear();
	_rest.clear();

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);


	// vertex positions -> attribute 0
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Normal vectors -> attribute 1
	glGenBuffers(1, &nbo_);
	glBindBuffer(GL_ARRAY_BUFFER, nbo_);
	glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(float), norm.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);

	// Colors buffer -> attribute 2
	glGenBuffers(1, &cbo_);
	glBindBuffer(GL_ARRAY_BUFFER, cbo_);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void World_Map::render_all_now() {
	while (!_not_spawned_cubes.empty()) {
		auto cube = _not_spawned_cubes.front();
		cube->y = cube->target_y;
		update_position(cube);

		_not_spawned_cubes.pop();

		rendered_triangles += 36;
	}

	for (auto moving : _moving_cubes) {
		moving->y = moving->target_y;
		update_position(moving);
	}

	_moving_cubes.clear();
}
