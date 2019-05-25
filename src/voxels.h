/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#ifndef RANDOMTILEMAPGENERATION_VOXELS_H
#define RANDOMTILEMAPGENERATION_VOXELS_H

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include "gl.h"
#include "stacktrace.h"
#include "utils.h"
#include "glmath.h"


struct chunk_t {
    int id;
    int contentSize;
    int childrenSize;
    long end;
};

struct Voxel {
public:
	Voxel();

	Voxel(int8_t x, int8_t y, int8_t z, int8_t color);

	int8_t x, y, z, color;

	vec3 get_color();
};

int32_t ReadInt(FILE *fp );

int8_t ReadByte( FILE *fp );

void ReadChunk( FILE *fp, chunk_t &chunk );

std::vector<Voxel> ReadVox(std::string filename);

void WriteVox(std::string filename, unsigned long X, unsigned long Y, unsigned long Z, std::vector<Voxel> voxels);

vec3 ColorToVec(const long color);

#endif //RANDOMTILEMAPGENERATION_VOXELS_H
