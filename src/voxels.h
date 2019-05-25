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

/**
 * Voxel structure, store rgb colors and position of a specific voxel
 */
struct Voxel {
public:
	Voxel();

	Voxel(int8_t x, int8_t y, int8_t z, int8_t color);

	int8_t x, y, z, color;

	vec3 get_color();
};

/**
 * Store one chunk of a .vox file
 */
struct chunk_t {
	int id;
	int contentSize;
	int childrenSize;
	long end;
};

/**
 * Read a 32bits binary number in a file
 * @param fp the file
 * @return the integer read
 */
int32_t ReadInt(FILE *fp );

/**
 * Read a 8bits binary number in a file
 * @param fp the file
 * @return the integer read
 */
int8_t ReadByte( FILE *fp );

/**
 * Read a chunk in a .vox file
 * @param fp the file
 * @return the chunk struct
 */
void ReadChunk( FILE *fp, chunk_t &chunk );

/**
 * Read a .vox file and return the list of voxels and their positions in a vector
 * @param filename
 * @return list of voxels and their positions in a vector
 */
std::vector<Voxel> ReadVox(std::string filename);

/**
 * Write a .vox file from a list of voxels
 * @param filename
 * @param X
 * @param Y
 * @param Z
 * @param voxels
 */
void WriteVox(std::string filename, unsigned long X, unsigned long Y, unsigned long Z, std::vector<Voxel> voxels);

/**
 * Convert a 8bits color in RGB
 * @param color
 * @return the color
 */
vec3 ColorToVec(const long color);

#endif //RANDOMTILEMAPGENERATION_VOXELS_H
