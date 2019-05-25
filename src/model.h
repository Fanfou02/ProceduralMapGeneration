/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#ifndef WFCTILEMAPGENERATION_MODEL_H
#define WFCTILEMAPGENERATION_MODEL_H

#include <pugixml.hpp>
#include <map>
#include <iostream>
#include "utils.h"
#include "gl.h"
#include "voxels.h"
#include <array>
#include <boost/algorithm/string.hpp>

#define STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION


/**
 * C++ Simple Tiled model
 * Use the WFC algorithm to generate a 3D simple tiled model from
 * a set of vox files (MagicaVoxel extension).
 *
 * Adapted from C# implementation: https://bitbucket.org/mxgmn/basic3dwfc/src/master/
 *
 */

class Model {
private:

    enum ObserveStates {
        True, False, Unfinished
    };

    Array4D<bool> wave;
    Array3D<bool> changes;
    Array3D<int> observed;
    std::vector<double> stationary;

    int FMX, FMY, FMZ, T, ground, bounds;
    bool periodic;

    std::vector<double> logProb;
    double logT;
    std::function<double()> random;

    Array3D<bool> propagator;

    std::vector<std::string> tilenames;
    std::vector<std::vector<Voxel>> voxeltiles;

    int voxelsize;

    std::vector<Voxel> rotateVoxels(std::vector<Voxel> array, size_t size);

    ObserveStates Observe();

    bool Propagate();

    void Clear();

public:
    Model(std::string name, int FMX, int FMY, int FMZ, bool periodic, std::string groundName);

    bool Run(int seed);

    void saveVoxelOutput(std::string filename);
};

#endif