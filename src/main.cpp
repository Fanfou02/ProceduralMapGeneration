//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#include "model.h"
//=============================================================================

int main(int argc, char *argv[])
{
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
            for (int k = 0; k < 20; k++) {

                int seed = rand();
                std::cout << "Seed :" << seed << std::endl;
                bool finished = model.Run(seed);
                if (finished)
                {
                    std::string filename = "test_" + std::to_string(i) + std::to_string(k) + ".vox";
                    std::cout << "Finished ! Generated vox file " << filename << std::endl;
                    model.saveVoxelOutput(filename);
                }
                else {
                    std::cout << "Contradiction occurred !" << std::endl;
                }
            }
        }

    }

    //Solar_viewer window("Solar Sytem", 640, 480);
    //return window.run();

}
