//
// Created by zyuiop on 18/05/19.
//

#include "World_Viewer.h"

World_Viewer::World_Viewer(const char* _title, int _width, int _height, std::vector<Voxel> voxels): GLFW_window(_title, _width, _height) {
	_voxels = voxels;
}

void World_Viewer::initialize() {

}

void World_Viewer::resize(int width, int height) {

}

void World_Viewer::paint() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int World_Viewer::run() {
	return GLFW_window::run();
}
