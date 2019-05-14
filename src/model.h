#ifndef WFCTILEMAPGENERATION_MODEL_H
#define WFCTILEMAPGENERATION_MODEL_H

#include <pugixml.hpp>
#include <map>
#include <iostream>
#include "utils.h"
#include "gl.h"
#include <png.h>
#include <array>
#include <boost/algorithm/string.hpp>
#include <bitmap_image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#undef STB_IMAGE_IMPLEMENTATION

struct RGBA
{
    uint8_t r, g, b, a;
};

class Model{
    private:
    bool**** wave;
    bool*** changes;
    int*** observed;
    double* stationary;

    //Random random;
    int FMX, FMY, FMZ, T, ground;
    bool periodic;

    double * logProb;
    double logT;
    std::function<double()> random;
    bool*** propagator;

    std::vector<std::vector<RGBA>> tiles;
    std::vector<std::string> tilenames;
    std::vector<std::vector<Voxel>> voxeltiles;

    int pixelsize, voxelsize;

    std::vector<RGBA> rotate(const std::vector<RGBA>& in_tile, const size_t tile_size)
    {
        std::vector<RGBA> out_tile;
        for (size_t y = 0; y < tile_size; ++y) {
            for (size_t x = 0; x < tile_size; ++x) {
                out_tile.push_back(in_tile[tile_size - 1 - y + x * tile_size]);
            }
        }
        return out_tile;
    }

    std::vector<Voxel> rotateVoxels(std::vector<Voxel> array, size_t size){
        for(Voxel voxel : array){
            voxel = Voxel(size-1-voxel.y, voxel.x, voxel.z, voxel.color);
        }
        return array;
    }

    std::vector<RGBA> loadTile(std::string filename){
        int width, height, comp;
        RGBA* rgba = reinterpret_cast<RGBA*>(stbi_load(filename.c_str(), &width, &height, &comp, 4));
        const auto num_pixels = width * height;
        std::vector<RGBA> tile(rgba, rgba + num_pixels);
        stbi_image_free(rgba);
        return tile;
    }


    bool Observe()
    {
        double min = 1E+3, sum, mainSum, logSum, noise, entropy;
        int argminx = -1, argminy = -1, argminz = -1, amount;
        bool* w;

        for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++) for (int z = 0; z < FMZ; z++)
                {
                    w = wave[x][y][z];
                    amount = 0;
                    sum = 0;

                    for (int t = 0; t < T; t++) if (w[t])
                        {
                            amount += 1;
                            sum += stationary[t];
                        }

                    if (sum == 0) return false;

                    noise = 1E-6 * random();

                    if (amount == 1) entropy = 0;
                    else if (amount == T) entropy = logT;
                    else
                    {
                        mainSum = 0;
                        logSum = std::log(sum);
                        for (int t = 0; t < T; t++) if (w[t]) mainSum += stationary[t] * logProb[t];
                        entropy = logSum - mainSum / sum;
                    }

                    if (entropy > 0 && entropy + noise < min)
                    {
                        min = entropy + noise;
                        argminx = x;
                        argminy = y;
                        argminz = z;
                    }
                }

        if (argminx == -1 && argminy == -1 && argminz == -1)
        {
            for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++) for (int z = 0; z < FMZ; z++) for (int t = 0; t < T; t++) if (wave[x][y][z][t])
                            {
                                observed[x][y][z] = t;
                                break;
                            }

            return true;
        }

        std::vector<double> distribution;
        for (int t = 0; t < T; t++) distribution[t] = wave[argminx][argminy][argminz][t] ? stationary[t] : 0;
        size_t r = spin_the_bottle(std::move(distribution), random());
        for (int t = 0; t < T; t++) wave[argminx][argminy][argminz][t] = t == r;
        changes[argminx][argminy][argminz] = true;


        return false;
    }

    bool Propagate()
    {
        bool change = false, b;
        for (int x2 = 0; x2 < FMX; x2++) for (int y2 = 0; y2 < FMY; y2++) for (int z2 = 0; z2 < FMZ; z2++) for (int d = 0; d < 6; d++)
                    {
                        int x1 = x2, y1 = y2, z1 = z2;
                        if (d == 0)
                        {
                            if (x2 == 0)
                            {
                                if (!periodic) continue;
                                else x1 = FMX - 1;
                            }
                            else x1 = x2 - 1;
                        }
                        else if (d == 1)
                        {
                            if (y2 == FMY - 1)
                            {
                                if (!periodic) continue;
                                else y1 = 0;
                            }
                            else y1 = y2 + 1;
                        }
                        else if (d == 2)
                        {
                            if (x2 == FMX - 1)
                            {
                                if (!periodic) continue;
                                else x1 = 0;
                            }
                            else x1 = x2 + 1;
                        }
                        else if (d == 3)
                        {
                            if (y2 == 0)
                            {
                                if (!periodic) continue;
                                else y1 = FMY - 1;
                            }
                            else y1 = y2 - 1;
                        }
                        else if (d == 4)
                        {
                            if (z2 == FMZ - 1)
                            {
                                if (!periodic) continue;
                                else z1 = 0;
                            }
                            else z1 = z2 + 1;
                        }
                        else
                        {
                            if (z2 == 0)
                            {
                                if (!periodic) continue;
                                else z1 = FMZ - 1;
                            }
                            else z1 = z2 - 1;
                        }

                        if (!changes[x1][y1][z1]) continue;

                        bool* w1 = wave[x1][y1][z1];
                        bool* w2 = wave[x2][y2][z2];

                        for (int t2 = 0; t2 < T; t2++) if (w2[t2])
                            {
                                bool* prop = propagator[d][t2];
                                b = false;

                                for (int t1 = 0; t1 < T && !b; t1++) if (w1[t1]) b = prop[t1];
                                if (!b)
                                {
                                    w2[t2] = false;
                                    changes[x2][y2][z2] = true;
                                    change = true;
                                }
                            }
                    }

        return change;
    }

    public:
    Model(std::string name, std::string subsetName, int FMX, int FMY, int FMZ, bool periodic, std::string groundName){
        this->FMX = FMX;
        this->FMY = FMY;
        this->FMZ = FMZ;
        this->periodic = periodic;
        this->ground = -1;

        pugi::xml_document xdoc;
        std::string filename = "../" + name + "/data.xml";
        if(!xdoc.load_file(filename.c_str())){
            std::cout<<"Error during opening of samples.xml";
        }
        pugi::xml_node xnode = xdoc.first_child();

        pixelsize = xnode.attribute("pixelsize").as_int();
        voxelsize =  xnode.attribute("voxelsize").as_int();
        xnode = xnode.first_child();

        std::vector<double> tempStationary;
        std::vector<int*> action;
        std::map<std::string, int> firstOccurrence;

        for (pugi::xml_node xtile: xnode.children()){
            std::string tilename = xtile.attribute("name").as_string();


            std::string sym = xtile.attribute("symmetry").as_string();
            int cardinality = 1;
            int (*a)(int i) = [](int i){ return i; };
            int (*b)(int i)  = [](int i){ return i; };

            if (!sym.compare("L"))
            {
                cardinality = 4;
                a = [](int i){ return (i + 1) % 4; };
                b = [](int i){ return i % 2 == 0 ? i + 1 : i - 1; };
            }
            else if (!sym.compare("T"))
            {
                cardinality = 4;
                a = [](int i){ return (i + 1) % 4; };
                b = [](int i){ return i % 2 == 0 ? i : 4 - i; };
            }
            else if (!sym.compare("I"))
            {
                cardinality = 2;
                a = [](int i){ return 1 - i; };
                b = [](int i){ return i; };
            }
            else if (!sym.compare("\\"))
            {
                cardinality = 2;
                a = [](int i){ return 1 - i; };
                b = [](int i){ return 1 - i; };
            }

            T = action.size();
            firstOccurrence[tilename] =  T;
            if (tilename == groundName) ground = T;

            std::cout << "Mapping" << std::endl;
            int ** map = new int*[cardinality];
            for (int t = 0; t < cardinality; t++)
            {
                map[t] = new int[8];
                map[t][0] = t;
                map[t][1] = a(t);
                map[t][2] = a(a(t));
                map[t][3] = a(a(a(t)));
                map[t][4] = b(t);
                map[t][5] = b(a(t));
                map[t][6] = b(a(a(t)));
                map[t][7] = b(a(a(a(t))));

                for (int s = 0; s < 8; s++) map[t][s] += T;
                std::cout << map[t] << std::endl;
                action.push_back(map[t]);
            }

            std::cout << "Opening: " << "../" << name << "/" << tilename << ".png" << std::endl;
            const std::vector<RGBA> bitmap = loadTile("../" + name + "/" + tilename + ".png" );
            std::cout << "Opening: " << "../" << name << "/" << tilename << ".vox" << std::endl;
            std::vector<Voxel> voxeltile = ReadVox("../" + name + "/" + tilename + ".vox");


            tiles.push_back(bitmap);
            tilenames.push_back(tilename + " 0");
            voxeltiles.push_back(voxeltile);

            for (int t = 1; t < cardinality; t++)
            {
                tiles.push_back(rotate(tiles[T + t - 1], pixelsize));
                tilenames.push_back("" + tilename + " " + std::to_string(t));
                voxeltiles.push_back(rotateVoxels(voxeltiles.at(T + t - 1), voxelsize));
            }

            for (int t = 0; t < cardinality; t++) tempStationary.push_back(xtile.attribute("weight").as_double());

        }

        T = action.size();
        stationary = &tempStationary[0];
        propagator = new bool**[6];

        for (int d = 0; d < 6; d++)
        {
            propagator[d] = new bool*[T];
            for (int t = 0; t < T; t++)
                propagator[d][t] = new bool[T];
        }

        wave = new bool***[FMX];
        changes = new bool**[FMX];
        observed = new int**[FMX];
        for (int x = 0; x < FMX; x++)
        {
            wave[x] = new bool**[FMY];
            changes[x] = new bool*[FMY];
            observed[x] = new int*[FMY];
            for (int y = 0; y < FMY; y++)
            {
                wave[x][y] = new bool*[FMZ];
                changes[x][y] = new bool[FMZ];
                observed[x][y] = new int[FMZ];
                for (int z = 0; z < FMZ; z++)
                {
                    wave[x][y][z] = new bool[T];
                    observed[x][y][z] = -1;
                }
            }
        }

        for (pugi::xml_node xneighbor : xnode.next_sibling().children())
        {

            std::string text = xneighbor.attribute("left").as_string();
            std::vector<std::string> left;
            boost::split(left, text , [](char c){return c == ' ';});

            text = xneighbor.attribute("right").as_string();
            std::vector<std::string> right;
            boost::split(right, text , [](char c){return c == ' ';});

            int L = action[firstOccurrence[left[0]]][left.size() == 1 ? 0 : std::atoi(left[1].c_str())];
            int D = action[L][1];
            int R = action[firstOccurrence[right[0]]][right.size() == 1 ? 0 : std::atoi(right[1].c_str())];
            int U = action[R][1];

            if (strncmp(xneighbor.name(),"horizontal", 10)  == 0)
            {
                propagator[0][R][L] = true;
                propagator[0][action[R][6]][action[L][6]] = true;
                propagator[0][action[L][4]][action[R][4]] = true;
                propagator[0][action[L][2]][action[R][2]] = true;

                propagator[1][U][D] = true;
                propagator[1][action[D][6]][action[U][6]] = true;
                propagator[1][action[U][4]][action[D][4]] = true;
                propagator[1][action[D][2]][action[U][2]] = true;
            }
            else for (int g = 0; g < 8; g++) propagator[4][action[L][g]][action[R][g]] = true;
        }

        for (int t2 = 0; t2 < T; t2++) for (int t1 = 0; t1 < T; t1++)
            {
                propagator[2][t2][t1] = propagator[0][t1][t2];
                propagator[3][t2][t1] = propagator[1][t1][t2];
                propagator[5][t2][t1] = propagator[4][t1][t2];
            }

    }


bool Run(int seed)
    {
        logT = std::log(T);
        logProb = new double[T];
        for (int t = 0; t < T; t++) logProb[t] = std::log(stationary[t]);

        Clear();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        random = [&]() { return dis(gen); };

        int ite = 0;
        while (ite++ < 100)
        {
            std::cout << ite << std::endl;
            bool result = Observe();
            if (result != false) return (bool)result;
            while (Propagate());
        }
        return true;
    }

    void Clear()
    {
        for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++) for (int z = 0; z < FMZ; z++)
                {
                    for (int t = 0; t < T; t++) wave[x][y][z][t] = true;
                    changes[x][y][z] = false;
                }

        if (ground >= 0)
        {
            for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++)
                {
                    for (int t = 0; t < T; t++) if (t != ground) wave[x][y][FMZ - 1][t] = false;
                    changes[x][y][FMZ - 1] = true;

                    for (int z = 0; z < FMZ - 1; z++)
                    {
                        wave[x][y][z][ground] = false;
                        changes[x][y][z] = true;
                    }
                }
        }
    }


    #pragma unmanaged
    bitmap_image PixelOutput() {
        bitmap_image result(FMX * pixelsize,FMY * pixelsize * FMZ + FMZ);
        int* bitmapData = new int[result.width()*result.height()];

        for (int z = 0; z < FMZ; z++) for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++)
                {
                    std::vector<RGBA> tile = tiles[observed[x][y][z]];
                    for (int yt = 0; yt < pixelsize; yt++) for (int xt = 0; xt < pixelsize; xt++)
                        {
                            RGBA c = tile[xt + yt * pixelsize];
                            result.set_pixel(x * pixelsize + xt, (y * pixelsize + yt) * FMX * pixelsize + (z * FMY * pixelsize + z) * (FMX * pixelsize), c.r, c.g, c.b);
                            bitmapData[x * pixelsize + xt + (y * pixelsize + yt) * FMX * pixelsize + (z * FMY * pixelsize + z) * (FMX * pixelsize)] =
                                    (int)0xff000000 | (c.r << 16) | (c.g << 8) | c.b;
                        }
                }
        result.save_image("test.bmp");

        return result;
    }
/*
std::string TextOutput()
    {
        var result = new System.Text.StringBuilder();

        for (int z = 0; z < FMZ; z++)
        {
            for (int y = 0; y < FMY; y++)
            {
                for (int x = 0; x < FMX; x++) result.Append($"{tilenames[observed[x][y][z]]}, ");
                result.Append(Environment.NewLine);
            }

            result.Append(Environment.NewLine);
        }

        return result.ToString();
    }

Tuple<int, int, int, Voxel[]> VoxelOutput()
    {
        List<Voxel> result = new List<Voxel>();

        for (int x = 0; x < FMX; x++) for (int y = 0; y < FMY; y++) for (int z = 0; z < FMZ; z++)
                {
                    Voxel[] voxeltile = voxeltiles[observed[x][FMY - y - 1][FMZ - z - 1]];
                    foreach (Voxel v in voxeltile) result.Add(new Voxel { x = (byte)(v.x + x * voxelsize), y = (byte)(v.y + y * voxelsize),
                            z = (byte)(v.z + z * voxelsize), color = v.color });
                }

        return new Tuple<int, int, int, Voxel[]>(FMX * voxelsize, FMY * voxelsize, FMZ * voxelsize, result.ToArray());
    }
*/
};

#endif