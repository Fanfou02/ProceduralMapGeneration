//
// Created by François Quellec on 11.05.19.
//

#ifndef WFCTILEMAPGENERATION_UTILS_H
#define WFCTILEMAPGENERATION_UTILS_H

#include <iostream>
#include <fstream>
#include <bitmap_image.hpp>
#include <random>
#include <vector>
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

std::vector<Voxel> ReadVox(std::string filename)
{
    std::vector<Voxel> voxels;
    std::fstream voxFile(filename, std::ios_base::in);
    if (voxFile.is_open()) {

        //std::string magic = new string(stream.ReadChars(4));
        //int version = stream.ReadInt32();
        char magic[5];
        voxFile.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        std::cout << "magic: " << magic << std::endl;
        u_int32_t version;
        voxFile.read(reinterpret_cast<char *>(&version), sizeof(version));
        std::cout << "version: " << version << std::endl;
        while(!voxFile.eof()){
            char chunkId[4];
            voxFile.read(reinterpret_cast<char *>(&chunkId), sizeof(chunkId));
            std::cout << "chunkId: " << chunkId << std::endl;

            u_int32_t chunkSize;
            voxFile.read(reinterpret_cast<char *>(&chunkSize), sizeof(chunkSize));
            std::cout << "chunkSize: " << chunkSize << std::endl;

            u_int32_t childChunk;
            voxFile.read(reinterpret_cast<char *>(&childChunk), sizeof(childChunk));
            std::cout << "childChunk: " << childChunk << std::endl;

            std::string chunkName = chunkId;

            if(chunkName.compare("SIZE")==0){
                std::cout << "SIZE" << std::endl;

                u_int32_t X;
                voxFile.read(reinterpret_cast<char *>(&X), sizeof(X));

                u_int32_t Y;
                voxFile.read(reinterpret_cast<char *>(&Y), sizeof(Y));

                u_int32_t Z;
                voxFile.read(reinterpret_cast<char *>(&Z), sizeof(Z));

                //voxFile.ignore(chunkSize - 4 * 3);
            } else if(chunkName.compare("XYZI")==0){
                std::cout << "XYZI" << std::endl;
                u_int32_t numVoxels;
                voxFile.read(reinterpret_cast<char *>(&numVoxels), sizeof(numVoxels));

                std::cout << "numVoxels: " << numVoxels << std::endl;

                for (int i = 0; i < numVoxels; ++i) {
                    int8_t X;
                    voxFile.read(reinterpret_cast<char *>(&X), sizeof(X));

                    int8_t Y;
                    voxFile.read(reinterpret_cast<char *>(&Y), sizeof(Y));

                    int8_t Z;
                    voxFile.read(reinterpret_cast<char *>(&Z), sizeof(Z));

                    int8_t color;
                    voxFile.read(reinterpret_cast<char *>(&Z), sizeof(Z));

                    voxels.push_back(Voxel(X, Y, Z, color));
                }

            }
        }

    }
    return voxels;
}




// Pick a random index weighted by a
size_t spin_the_bottle(const std::vector<double>& a, double between_zero_and_one)
{
    double sum = std::accumulate(a.begin(), a.end(), 0.0);

    if (sum == 0.0) {
        return std::floor(between_zero_and_one * a.size());
    }

    double between_zero_and_sum = between_zero_and_one * sum;

    double accumulated = 0;

    for (unsigned long i = 0; i < a.size(); ++i) {
        accumulated += a[i];
        if (between_zero_and_sum <= accumulated) {
            return i;
        }
    }

    return 0;
}

#endif //WFCTILEMAPGENERATION_UTILS_H
