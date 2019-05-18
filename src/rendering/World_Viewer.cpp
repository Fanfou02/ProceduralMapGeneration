#include <utility>

//
// Created by zyuiop on 18/05/19.
//

#include "World_Viewer.h"
#include "voxels.h"

World_Viewer::World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels) : GLFW_window(_title,
																												 _width,
																												 _height) {
	_voxels = std::move(voxels);
}

void World_Viewer::initialize() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_shader.load(SHADER_PATH   "/cube.vert", SHADER_PATH   "/cube.frag");

	for (Voxel v : _voxels) {
		v.initialize();
	}
}

void World_Viewer::resize(int width, int height) {
	width_  = width;
	height_ = height;
	glViewport(0, 0, width, height);

	std::cout << "Resize " << width << " " << height << std::endl;
}

void World_Viewer::paint() {
	glClear(GL_COLOR_BUFFER_BIT);

	/*vec4 center = vec4(10, 10, 10, 0);
	vec4 eye = vec4(5, 5, 5, 1.0);
	vec4 up = vec4(0, 1, 0, 0);

	auto fovy_ = 45;
	auto near_ = 0.01f;
	auto far_  = 20;

	mat4 view = mat4::look_at(vec3(eye), vec3(center), vec3(up));
	mat4 projection = mat4::perspective(fovy_, (float) width_ / (float) height_, near_, far_);

	// the matrices we need: model, modelview, modelview-projection, normal
	mat4 m_matrix;
	mat4 mv_matrix;
	mat4 mvp_matrix;
*/
	// render cubes
	for (Voxel v : _voxels) {
		/*mv_matrix = view * m_matrix;
		mvp_matrix = projection * mv_matrix;*/
		_shader.use();
		//_shader.set_uniform("modelview_projection_matrix", vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//_shader.set_uniform("color", vec4(1.0f, 0.5f, 0.2f, 1.0f));
		v.draw();
	}

}

int World_Viewer::run() {
	return GLFW_window::run();
}
