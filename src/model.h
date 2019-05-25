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

    /**
     * Perform a 90deg Rotate of a block of voxels on the x-y coordinates
     * @param array the vector containing all the voxels of the block
     * @param size the width=height=depth of the block
     * @return the rotated block of voxels
     */
    std::vector<Voxel> rotateVoxels(std::vector<Voxel> array, size_t size);

    /**
     * Look for a block to be randomly assigned among a set of "authorized" blocks
     * If all block have been assigned return true
     * If a block have 0 authorized assignation there is a contradiction, return false
     * If a block have been correctly assigned but there is still blocks to assign return Unfinished
     */
    ObserveStates Observe();

    /**
     * Update the list of possible assignation of each blocks depending on the newly assigned block and the rules.
     */
    bool Propagate();

    /**
     * Set the constraints of the model, before the launch of the algorithm
     * For example, the bounds, the ground.
     */
    void Clear();

public:
    Model(std::string name, int FMX, int FMY, int FMZ, bool periodic, std::string groundName);

    /**
     * Run an instance of the Simple Tile model with a specific seed for randomness
     * @param seed
     * @return true if the model have been created, false if a contradiction occurs
     */
    bool Run(int seed);

    /**
     * Save the last generated model into a .vox file (MagicaVoxel extension)
     * @param filename name of the file we want to save the model
     */
    void saveVoxelOutput(std::string filename);
};

#endif