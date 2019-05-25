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
	static const float vertices[];

public:
	int x, target_y, z;
	float y;

	GLintptr offset = -1;

	vec3 color;
	vec4 pos_;
	World_Map *worldMap;

	float speed = 0;

	bool timer(float seconds);

	Cube(int8_t x, int8_t y, int8_t z, vec3 color, World_Map *map);
	void add_to_chunk(std::vector<float> &vert, std::vector<float> &norm, std::vector<float> &colors);
	void add_position_to_chunk(std::vector<float> &vert);

};


#endif //RANDOMTILEMAPGENERATION_CUBE_H
