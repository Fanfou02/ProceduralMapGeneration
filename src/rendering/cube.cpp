//
// Created by zyuiop on 19/05/19.
//

#include "cube.h"


Cube::Cube(int8_t x, int8_t y, int8_t z, int8_t color) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->color = color;
	this->pos_ = vec4(x / 10.0, y / 10.0, z / 10.0, 1.0);
}

void Cube::draw(GLenum mode) {
	if (vao_ == 0) initialize();

	glBindVertexArray(vao_);
	glDrawElements(mode, 8, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Cube::initialize() {
	float vertices[] = {
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0
	};

	GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
			4, 0, 3,
			4, 3, 7,
			1, 0, 4,
			1, 4, 5,
			2, 1, 5,
			2, 5, 6,
			7, 3, 2,
			7, 2, 6,
			5, 4, 7,
			5, 7, 6
	};

	glGenVertexArrays(1, &vao_);

	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ibo_);

	glBindVertexArray(vao_);


	// vertex positions -> attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}