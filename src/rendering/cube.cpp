//
// Created by zyuiop on 19/05/19.
//

#include <voxels.h>
#include "cube.h"
#include "World_Map.h"

Cube::Cube(int8_t x, int8_t y, int8_t z, vec4 color, World_Map *map) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->color = color;
	this->pos_ = vec4(x, y, z, 1.0);
	this->worldMap = map;
}

const float Cube::vertices[24] = {
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5
};

const GLuint Cube::faces[6][6] = {
		{
				0, 1, 2,
				0, 2, 3
		},
		{
				5, 4, 7,
				5, 7, 6
		},
		{
				4, 0, 3,
				4, 3, 7
		},
		{
				2, 1, 5,
				2, 5, 6
		},
		{
				1, 0, 4,
				1, 4, 5
		},
		{
				7, 3, 2,
				7, 2, 6
		}
};

void Cube::draw(GLenum mode) {
	if (vao_ == 0) initialize();
	if (triangles == 0) return;

	glBindVertexArray(vao_);
	glDrawElements(mode, triangles, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Cube::add_rectangle(std::vector<GLuint> &vec, GLuint const *coords) {
	for (int i = 0; i < 6; ++i)
		vec.push_back(coords[i]);

	triangles += 6;
}

void Cube::initialize() {
	std::vector<GLuint> indices;

	if (!worldMap->has_block(x, y, z + 1)) {
		// Front
		add_rectangle(indices, faces[FRONT]);
	}

	if (!worldMap->has_block(x, y, z - 1)) {
		// Back
		add_rectangle(indices, faces[BACK]);
	}

	if (!worldMap->has_block(x + 1, y, z)) {
		// Left
		add_rectangle(indices, faces[LEFT]);
	}

	if (!worldMap->has_block(x - 1, y, z)) {
		// Right
		add_rectangle(indices, faces[RIGHT]);
	}

	if (!worldMap->has_block(x, y + 1, z)) {
		// Top
		add_rectangle(indices, faces[TOP]);
	}

	if (!worldMap->has_block(x, y - 1, z)) {
		// Bottom
		add_rectangle(indices, faces[BOTTOM]);
	}

	glGenVertexArrays(1, &vao_);

	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ibo_);

	glBindVertexArray(vao_);


	// vertex positions -> attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::vertices), Cube::vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}