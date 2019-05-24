//
// Created by zyuiop on 19/05/19.
//

#include <voxels.h>
#include "cube.h"
#include "World_Map.h"
#define GRAVITY -9.81

Cube::Cube(int8_t x, int8_t y, int8_t z, vec3 color, World_Map *map) {
	this->target_y = y;

	this->x = x;
	this->y = y + 100;
	this->z = z;

	this->color = color;
	this->pos_ = vec4(x, y, z, 1.0);
	this->worldMap = map;
}

bool Cube::timer(float seconds) {
	if (y < target_y) {
		y = target_y;
		return false;
	} else if (y == target_y) {
		return false;
	}

	speed = speed + (seconds * GRAVITY);
	y = y + speed * seconds;

	if (y < target_y)
		y = target_y;

	worldMap->update_position(this);
	return true;
}

const float Cube::vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void Cube::add_to_chunk(std::vector<float> &vert, std::vector<float> &norm, std::vector<float> &colors) {
	for (int i = 0; i < 36; ++i) {
		vert.push_back(vertices[6 * i] + x);
		vert.push_back(vertices[6 * i + 1] + y);
		vert.push_back(vertices[6 * i + 2] + z);

		norm.push_back(vertices[6 * i + 3]);
		norm.push_back(vertices[6 * i + 4]);
		norm.push_back(vertices[6 * i + 5]);

		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}
}

void Cube::add_position_to_chunk(std::vector<float> &vert) {
	for (int i = 0; i < 36; ++i) {
		vert.push_back(vertices[6 * i] + x);
		vert.push_back(vertices[6 * i + 1] + y);
		vert.push_back(vertices[6 * i + 2] + z);
	}
}