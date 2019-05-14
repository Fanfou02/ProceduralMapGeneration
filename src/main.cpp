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

    std::vector<Voxel> voxs = ReadVox("../Knots/up.vox");
    std::cout << voxs.size();
    WriteVox("up.vox", 3, 3, 3, voxs);
    /*
     pugi::xml_document xdoc;
    if(!xdoc.load_file("../samples.xml")){
        std::cout<<"Error during opening of samples.xml";
        return -1;
    }




    for (pugi::xml_node sample: xdoc.child("samples").children("sample"))
    {
        std::string name = sample.attribute("name").as_string();
        std::cout <<"Sample: " << name << std::endl;


        Model model =  Model(name, sample.attribute("subset").as_string(), sample.attribute("X").as_int(), sample.attribute("Y").as_int(), sample.attribute("Z").as_int(),
                                sample.attribute("periodic"), sample.attribute("ground").as_string());


        for (int i = 0; i < sample.attribute("screenshots").as_int(); i++) {
            for (int k = 0; k < 10; k++) {

                int seed = rand();
                std::cout << "Seed :" << seed << std::endl;
                bool finished = model.Run(seed);
                if (finished)
                {
                    std::cout << "Finished !" << std::endl;
                    model.PixelOutput().save_image("test" + std::to_string(k) + ".bmp");
                    std::cout << model.PixelOutput().height() << " " << model.PixelOutput().width() << std::endl;
                }
                else {
                    std::cout << "contradiction" << std::endl;
                }
            }
        }

    }

    //Solar_viewer window("Solar Sytem", 640, 480);
    //return window.run();
     * */
}
