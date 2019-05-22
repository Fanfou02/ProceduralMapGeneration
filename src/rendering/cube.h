//
// Created by zyuiop on 19/05/19.
//

#ifndef RANDOMTILEMAPGENERATION_CUBE_H
#define RANDOMTILEMAPGENERATION_CUBE_H

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <glmath.h>
#include "gl.h"
#include "stacktrace.h"
#include "utils.h"

class World_Map;

enum Faces {
	FRONT = 0, BACK = 1, LEFT = 2, RIGHT = 3, TOP = 4, BOTTOM = 5
};

class Cube {
private :
	static const float vertices[24];

	static const GLuint faces[6][6];
	static const float normals[6][3];

	void add_rectangle(std::vector<GLuint> &vec, GLuint const coords[]);
	void add_normal(std::vector<float> &vec, float const coords[]);

	int triangles = 0;

public:
	int8_t x, y, z;
	/// vertex array object
	GLuint vao_ = 0;
	/// vertex buffer object
	GLuint vbo_ = 0;
	/// normals buffer object
	GLuint nbo_ = 0;
	/// index buffer object
	GLuint ibo_ = 0;

	vec3 color;
	vec4 pos_;
	World_Map *worldMap;

	Cube(int8_t x, int8_t y, int8_t z, vec3 color, World_Map *map);

	void initialize();

	void draw(GLenum mode = GL_TRIANGLES);
};


#endif //RANDOMTILEMAPGENERATION_CUBE_H
