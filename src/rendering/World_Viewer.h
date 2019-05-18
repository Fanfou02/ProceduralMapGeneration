//
// Created by zyuiop on 18/05/19.
//

#ifndef RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
#define RANDOMTILEMAPGENERATION_WORLD_VIEWER_H


#include <utils.h>
#include <shader.h>
#include "glfw_window.h"
#include "voxels.h"

class World_Viewer : public GLFW_window {

	void initialize() override;

	void resize(int width, int height) override;

	void paint() override;

public:
	World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels);

	int run() override;

private:
	std::vector<Voxel> _voxels;
	Shader _shader;

	/// current viewport dimension
	int  width_, height_;
};


#endif //RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
