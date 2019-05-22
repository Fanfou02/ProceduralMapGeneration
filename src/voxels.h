//
// Created by zyuiop on 18/05/19.
//

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

	vec4 get_color();
};

int32_t ReadInt(FILE *fp );

int8_t ReadByte( FILE *fp );

void ReadChunk( FILE *fp, chunk_t &chunk );

std::vector<Voxel> ReadVox(std::string filename);

static void WriteVox(std::string filename, size_t X, size_t Y, size_t Z, std::vector<Voxel> voxels);

vec4 ColorToVec(const long color);

#endif //RANDOMTILEMAPGENERATION_VOXELS_H
