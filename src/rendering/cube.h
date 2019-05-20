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

class Cube {
private :
public:
	int8_t x, y, z;
	/// vertex array object
	GLuint vao_ = 0;
	/// vertex buffer object
	GLuint vbo_ = 0;
	/// index buffer object
	GLuint ibo_ = 0;

	vec4 color;
	vec4 pos_;
	World_Map *worldMap;

	Cube(int8_t x, int8_t y, int8_t z, vec4 color, World_Map *map);

	void initialize();

	void draw(GLenum mode=GL_TRIANGLES);
};


#endif //RANDOMTILEMAPGENERATION_CUBE_H
