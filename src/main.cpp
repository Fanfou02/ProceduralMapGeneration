//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#include <rendering/World_Viewer.h>
#include "model.h"
#include "solar_viewer.h"
#include "voxels.h"
//=============================================================================

int main(int argc, char *argv[]) {

	/* std::vector<std::array<int,     8>>  action;

	 std::cout << action.size() << std::endl;
	 pugi::xml_document xdoc;
	 if(!xdoc.load_file("../samples.xml")){
		 std::cout<<"Error during opening of samples.xml";
		 return -1;
	 }


	 for (pugi::xml_node sample: xdoc.child("samples").children("sample"))
	 {
		 std::string name = sample.attribute("name").as_string();
		 std::cout <<"Sample: " << name << std::endl;


		 Model model =  Model(name, sample.attribute("X").as_int(), sample.attribute("Y").as_int(), sample.attribute("Z").as_int(),
								 sample.attribute("periodic").as_bool(), sample.attribute("ground").as_string());


		 for (int i = 0; i < sample.attribute("screenshots").as_int(); i++) {
			 for (int k = 0; k < 10; k++) {

				 int seed = rand();
				 //std::cout << "Seed :" << seed << std::endl;
				 bool finished = model.Run(seed);
				 std::string filename = "test_" + std::to_string(i) + std::to_string(k) + ".vox";
				 if (finished)
				 {

					 std::cout << "Finished ! Generated vox file " << filename << std::endl;
					 model.saveVoxelOutput(filename);
				 }
				 else {
					 //std::cout << "Contradiction occurred ! " << std::endl;
				 }
			 }
		 }
	 }
 */
	// auto voxels = ReadVox("test_01.vox");
	std::vector<Voxel> voxels(5);
	voxels[1] = Voxel(3, 3, 0, 64);
	voxels[2] = Voxel(1, 1, 0, 64);
	voxels[3] = Voxel(-3, 0, 3, 64);
	voxels[4] = Voxel(-2, 0, 2, 64);
	voxels[0] = Voxel(-1, 0, 1, 64);
	voxels.emplace_back(0, 0, 0, 64);
	voxels.emplace_back(1, 0, 0, 64);
	voxels.emplace_back(0, 1, 0, 64);
	World_Viewer window("Solar Sytem", 640, 480, voxels);

	// Solar_viewer window("Solar System", 640, 480);

	return window.run();

}
