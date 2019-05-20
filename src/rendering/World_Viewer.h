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
#include "World_Map.h"

class World_Viewer : public GLFW_window {

	void initialize() override;

	void resize(int width, int height) override;

	void paint() override;

protected:
	void keyboard(int key, int scancode, int action, int mods) override;

public:
	World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels);

	~World_Viewer() override;
	int run() override;


private:
	World_Map *worldMap;
	vec4 position = vec4(0, 0, 0, 1.0);
	Shader _shader;

	/// current viewport dimension
	int  width_, height_;

	/// rotation in x direction around the planet/sun from the original point
	float x_angle_ = 0;
	/// rotation in y direction around the planet/sun from the original point
	float y_angle_ = 0;
};


#endif //RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
