//
// Created by zyuiop on 18/05/19.
//

#ifndef RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
#define RANDOMTILEMAPGENERATION_WORLD_VIEWER_H


#include <utils.h>
#include <shader.h>
#include "glfw_window.h"
#include "voxels.h"
#include "cube.h"

class World_Viewer : public GLFW_window {

	void initialize() override;

	void resize(int width, int height) override;

	void paint() override;

public:
	World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels);

	int run() override;

	virtual ~World_Viewer();

private:
	std::vector<Cube*> _cubes;
	Shader _shader;

	/// current viewport dimension
	int  width_, height_;

	/// rotation in x direction around the planet/sun from the original point
	float x_angle_ = 0;
	/// rotation in y direction around the planet/sun from the original point
	float y_angle_ = 0;
};


#endif //RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
