/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#ifndef WFCTILEMAPGENERATION_UTILS_H
#define WFCTILEMAPGENERATION_UTILS_H

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <gl.h>
#include "stacktrace.h"

/**
 * Print an error message in the logs and stp the process
 * @param info the message to print
 */
void Error(const char *info);

/**
 * Randomly pick an index of vector 
 * @param a the vector to rotate
 * @param between_zero_and_one the seed
 * @return the rotated vector
 */
size_t rotateVector(const std::vector<double> &a, double between_zero_and_one);

/**
 * Check that index is between 0 and bounds and generate an Error if not
 * @param index
 * @param bound
 */
void checkBounds(size_t index, size_t bound);


void MessageCallback(GLenum source,
					 GLenum type,
					 GLuint id,
					 GLenum severity,
					 GLsizei length,
					 const GLchar *message,
					 const void *userParam);

/**
 * 4D array, made with 1D vector for efficiency
 * @tparam T The type of the array
 */
template<typename T>
struct Array4D {
	size_t dim1, dim2, dim3, dim4;
	std::vector<T> data;

	size_t getIndex(size_t d1, size_t d2, size_t d3, size_t d4) {
		checkBounds(d1, dim1);
		checkBounds(d2, dim2);
		checkBounds(d3, dim3);
		checkBounds(d4, dim4);
		return d1 * dim2 * dim3 * dim4 + d2 * dim3 * dim4 + d3 * dim4 + d4;
	}

public:
	Array4D(size_t dim1, size_t dim2, size_t dim3, size_t dim4, T defaultValue) :
			dim1(dim1), dim2(dim2), dim3(dim3), dim4(dim4) {
		data = std::vector<T>(dim1 * dim2 * dim3 * dim4, defaultValue);
	}

	Array4D() {}

	T get(size_t d1, size_t d2, size_t d3, size_t d4) {
		return data.at(getIndex(d1, d2, d3, d4));
	}

	std::vector<T> get(size_t d1, size_t d2, size_t d3) {
		return std::vector<T>(&data[getIndex(d1, d2, d3, 0)], &data[getIndex(d1, d2, d3, dim4 - 1)]);
	}

	void set(size_t d1, size_t d2, size_t d3, size_t d4, T value) {
		data.at(getIndex(d1, d2, d3, d4)) = value;
	}
};

/**
 * 3D array, made with 1D vector for efficiency
 * @tparam T The type of the array
 */
template<typename T>
struct Array3D {
	size_t width, height, depht;
	std::vector<T> data;

	size_t getIndex(size_t x, size_t y, size_t z) {
		checkBounds(x, width);
		checkBounds(y, height);
		checkBounds(z, depht);
		return x * height * depht + y * depht + z;
	}

public:
	Array3D(size_t width, size_t height, size_t depht, T defaultValue) :
			width(width), height(height), depht(depht) {
		data = std::vector<T>(width * height * depht, defaultValue);
	}

	Array3D() {}

	T get(size_t x, size_t y, size_t z) {
		return data.at(getIndex(x, y, z));
	}

	void set(size_t x, size_t y, size_t z, T value) {
		data.at(getIndex(x, y, z)) = value;
	}

	std::vector<T> get(size_t d1, size_t d2) {
		return std::vector<T>(&data[getIndex(d1, d2, 0)], &data[getIndex(d1, d2, depht - 1)]);
	}
};

#endif //WFCTILEMAPGENERATION_UTILS_H
