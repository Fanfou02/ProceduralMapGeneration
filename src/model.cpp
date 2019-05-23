
#include "model.h"

void Model::saveVoxelOutput(std::string filename)
{
	std::vector<Voxel> result;
	for (int8_t x = 0; x < FMX; x++) for (int8_t y = 0; y < FMY; y++) for (int8_t z = 0; z < FMZ; z++)
			{

				int index = observed.get(x, FMY - y - 1, FMZ - z - 1);

				if(index >= 0) {
					std::vector<Voxel> voxeltile = voxeltiles[index];
					for (Voxel v : voxeltile)
						result.push_back(Voxel((v.x + x * voxelsize), (v.y + y * voxelsize), (v.z + z * voxelsize),
											   v.color));
				}
			}

	WriteVox(filename, FMX * voxelsize, FMY * voxelsize, FMZ * voxelsize, result);
}