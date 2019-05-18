//
// Created by François Quellec on 11.05.19.
//

#ifndef WFCTILEMAPGENERATION_UTILS_H
#define WFCTILEMAPGENERATION_UTILS_H

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include "stacktrace.h"


void Error( const char *info );

struct chunk_t {
    int id;
    int contentSize;
    int childrenSize;
    long end;
};

struct Voxel {
public :
    int8_t x, y, z, color;

    Voxel(int8_t x, int8_t y, int8_t z, int8_t color)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->color = color;
    }
};


int32_t ReadInt( FILE *fp );

int8_t ReadByte( FILE *fp );


void ReadChunk( FILE *fp, chunk_t &chunk );

std::vector<Voxel> ReadVox(std::string filename);

static void WriteVox(std::string filename, size_t X, size_t Y, size_t Z, std::vector<Voxel> voxels);

size_t spin_the_bottle(const std::vector<double>& a, double between_zero_and_one);

void checkBounds(size_t index, size_t bound);

template <typename T>
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
        data = std::vector<T>(dim1*dim2*dim3*dim4, defaultValue);
    }

    Array4D(){}

    T get(size_t d1, size_t d2, size_t d3, size_t d4){
        return data.at(getIndex(d1, d2, d3, d4));
    }

    std::vector<T> get(size_t d1, size_t d2, size_t d3){
        return std::vector<T>(&data[getIndex(d1, d2, d3, 0)], &data[getIndex(d1, d2, d3, dim4-1)]);
    }

    void set(size_t d1, size_t d2, size_t d3, size_t d4, T value){
        data.at(getIndex(d1, d2, d3, d4)) = value;
    }
};

template <typename T>
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
        data = std::vector<T>(width*height*depht, defaultValue);
    }

    Array3D(){}

    T get(size_t x, size_t y, size_t z){
        return data.at(getIndex(x, y, z));
    }

    void set(size_t x, size_t y, size_t z, T value){
        data.at(getIndex(x, y, z)) = value;
    }

    std::vector<T> get(size_t d1, size_t d2){
        return std::vector<T>(&data[getIndex(d1, d2, 0)], &data[getIndex(d1, d2, depht-1)]);
    }
};

#endif //WFCTILEMAPGENERATION_UTILS_H
