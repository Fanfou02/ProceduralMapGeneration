/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#include <rendering/World_Viewer.h>
#include "model.h"
#include "voxels.h"

int main(int argc, char *argv[]) {
	std::vector<std::array<int, 8>> action;

	std::cout << action.size() << std::endl;
	pugi::xml_document xdoc;
	if (!xdoc.load_file("../samples.xml")) {
		std::cout << "Error during opening of samples.xml";
		return -1;
	}

	srand(time(NULL));

	std::string filename;

	pugi::xml_node sample = xdoc.child("samples").child("sample");

	if (argc > 1) {
		filename = argv[1];
		std::cout << "loading immediately " << filename << std::endl;
	} else {


			std::string name = sample.attribute("name").as_string();
			std::cout << "Sample: " << name << std::endl;


			Model model = Model(name, sample.attribute("X").as_int(), sample.attribute("Y").as_int(),
								sample.attribute("Z").as_int(),
								sample.attribute("periodic").as_bool(), sample.attribute("ground").as_string());


			for (int i = 0; i < sample.attribute("screenshots").as_int(); i++) {
				for (int k = 0; k < 1000; k++) {

					int seed = rand();
					bool finished = model.Run(seed);
					filename = "test_" + std::to_string(seed) + ".vox";
					if (finished) {
						std::cout << "Finished ! Generated vox file " << filename << std::endl;
						model.saveVoxelOutput(filename);
                        break;
					}
				}
			}

	}


	auto voxels = ReadVox(filename);
	std::cout << voxels.size() << " voxels loaded" << std::endl;

	World_Viewer window("Wave Function Collapse", 640, 480, voxels, sample.attribute("groundLevel").as_int(10), sample.attribute("blockSize").as_int(10));

	std::cout << ColorToVec(0xFFFFFFFF) << std::endl;
	std::cout << ColorToVec(0x00000000) << std::endl;

	return window.run();

}
