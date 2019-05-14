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


void Error( const char *info ) {
    printf( "[Error] MV_VoxelModel :: %s\n", info );
}

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


int32_t ReadInt( FILE *fp ) {
    int32_t v = 0;
    fread( &v, 4, 1, fp );
    return v;
}

int8_t ReadByte( FILE *fp ) {
    int8_t v = 0;
    fread( &v, 1, 1, fp );
    return v;
}


void ReadChunk( FILE *fp, chunk_t &chunk ) {
    // read chunk
    chunk.id = ReadInt( fp );
    chunk.contentSize  = ReadInt( fp );
    chunk.childrenSize = ReadInt( fp );

    // end of chunk : used for skipping the whole chunk
    chunk.end = ftell( fp ) + chunk.contentSize + chunk.childrenSize;

    // print chunk info
    const char *c = ( const char * )( &chunk.id );
    printf( "[Log] MV_VoxelModel :: Chunk : %c%c%c%c : %d %d\n",
            c[0], c[1], c[2], c[3],
            chunk.contentSize, chunk.childrenSize
    );
}



// magic number
int MV_ID( int a, int b, int c, int d ) {
    return ( a ) | ( b << 8 ) | ( c << 16 ) | ( d << 24 );
}

const int MV_VERSION = 150;
const int ID_VOX  = MV_ID( 'V', 'O', 'X', ' ' );
const int ID_MAIN = MV_ID( 'M', 'A', 'I', 'N' );
const int ID_SIZE = MV_ID( 'S', 'I', 'Z', 'E' );
const int ID_XYZI = MV_ID( 'X', 'Y', 'Z', 'I' );
const int ID_RGBA = MV_ID( 'R', 'G', 'B', 'A' );
//https://bitbucket.org/volumesoffun/cubiquity/src/9e1cb814c24e/Tools/ProcessVDB/Dependencies/MagicaVoxelModel.h?at=master

std::vector<Voxel> ReadVox(std::string filename)
{
    std::vector<Voxel> voxels;

    FILE *voxFile = fopen(filename.c_str(), "rb");
    if (voxFile)
    {
        // magic number
        int magic = ReadInt( voxFile );
        if ( magic != ID_VOX ) {
            Error( "magic number does not match" );
            return voxels;
        }

        // version
        int version = ReadInt( voxFile );
        if ( version != MV_VERSION ) {
            Error( "version does not match" );
            return voxels;
        }

        // main chunk
        chunk_t mainChunk;
        ReadChunk( voxFile, mainChunk );
        if ( mainChunk.id != ID_MAIN ) {
            Error( "main chunk is not found" );
            return voxels;
        }

        // read children chunks
        while ( ftell( voxFile ) < mainChunk.end ) {
            // read chunk header
            chunk_t sub;
            ReadChunk( voxFile, sub );

            if ( sub.id == ID_SIZE ) {
                // size
                int sizex = ReadInt( voxFile );
                int sizey = ReadInt( voxFile );
                int sizez = ReadInt( voxFile );
                std::cout << "size : " << sizex << " " << sizey << " " << sizez << std::endl;
            }
            else if ( sub.id == ID_XYZI ) {
                // numVoxels
                int numVoxels = ReadInt( voxFile );
                if ( numVoxels < 0 ) {
                    Error( "negative number of voxels" );
                    return voxels;
                }

                // voxels
                if ( numVoxels > 0 ) {
                    for (int i = 0; i < numVoxels; ++i) {
                        int X = ReadByte(voxFile);

                        int Y = ReadByte(voxFile);;

                        int Z = ReadByte(voxFile);

                        int color = ReadByte(voxFile);

                        voxels.push_back(Voxel(X, Y, Z, color));
                    }
                }
            }

            // skip unread bytes of current chunk or the whole unused chunk
            fseek( voxFile, sub.end, SEEK_SET );
        }
        std::cout << "Voxels: " << voxels.size() << std::endl;

    }
    return voxels;
}

static void WriteVox(std::string filename, int X, int Y, int Z, std::vector<Voxel> voxels)
{

    FILE *voxFile = fopen(filename.c_str(), "w");

    int32_t version = 150;
    fwrite( &ID_VOX, 4, 1, voxFile );
    fwrite( &version, 4, 1, voxFile );

    int32_t main_contentSize = 0;
    int32_t main_childrenSize = 40 + voxels.size()*4;
    fwrite( &ID_MAIN, 4, 1, voxFile );
    fwrite( &main_contentSize, 4, 1, voxFile );
    fwrite( &main_childrenSize, 4, 1, voxFile );

    int32_t size_contentSize = 12;
    int32_t size_childrenSize = 0;
    fwrite( &ID_SIZE, 4, 1, voxFile );
    fwrite( &size_contentSize, 4, 1, voxFile );
    fwrite( &size_childrenSize, 4, 1, voxFile );
    fwrite( &X, 4, 1, voxFile );
    fwrite( &Y, 4, 1, voxFile );
    fwrite( &Z, 4, 1, voxFile );

    int32_t xyzi_contentSize = 4 + voxels.size() * 4;
    int32_t xyzi_childrenSize = 0;
    int32_t number_voxels = voxels.size();
    fwrite( &ID_XYZI, 4, 1, voxFile );
    fwrite( &xyzi_contentSize, 4, 1, voxFile );
    fwrite( &xyzi_childrenSize, 4, 1, voxFile );
    fwrite( &number_voxels, 4, 1, voxFile );

    for (Voxel v : voxels)
    {
        int8_t x = v.x;
        int8_t y = v.y;
        int8_t z = v.z;
        int8_t color = v.color;

        fwrite( &x, 1, 1, voxFile );
        fwrite( &y, 1, 1, voxFile );
        fwrite( &z, 1, 1, voxFile );
        fwrite( &color, 1, 1, voxFile );
    }
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
