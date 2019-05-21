#include <utility>

//
// Created by zyuiop on 18/05/19.
//

#include "World_Viewer.h"
#include "voxels.h"
#include "math.h"

World_Viewer::World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels) : GLFW_window(_title,
																												 _width,
																												 _height) {
	worldMap = new World_Map(voxels);
}

void World_Viewer::initialize() {
	// During init, enable debug output
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, nullptr);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_shader.load(SHADER_PATH   "/cube.vert", SHADER_PATH   "/cube.frag");
}

void World_Viewer::resize(int width, int height) {
	width_ = width;
	height_ = height;
	glViewport(0, 0, width, height);

	std::cout << "Resize " << width << " " << height << std::endl;
}

void World_Viewer::paint() {
	glClear(GL_COLOR_BUFFER_BIT);

	vec4 center = position;
	vec4 eye = vec4(0, 0, 1, 1.0);
	vec4 up = vec4(0, 1, 0, 0);

	eye = mat4::translate(center) * mat4::rotate_y(y_angle_) * mat4::rotate_x(x_angle_) * eye;
	up = mat4::rotate_y(y_angle_) * mat4::rotate_x(x_angle_) * up;



	auto fovy_ = 45;
	auto near_ = 0.1f;
	auto far_  = 20;

	mat4 view = mat4::look_at(vec3(eye), vec3(center), vec3(up));
	mat4 projection = mat4::perspective(fovy_, (float) width_ / (float) height_, near_, far_);

	// the matrices we need: model, modelview, modelview-projection, normal
	mat4 m_matrix;
	mat4 mv_matrix;
	mat4 mvp_matrix;

	_shader.use();
	int i = 0;
	// render cubes
	for (Cube* v : worldMap->_cubes) {
		m_matrix = mat4::translate(v->pos_);
		mv_matrix = view * m_matrix;
		mvp_matrix = projection * mv_matrix;

		_shader.set_uniform("modelview_projection_matrix", mvp_matrix);
		_shader.set_uniform("color", v->color);

		v->draw();
	}

	// check for OpenGL errors
	glCheckError();
}

int World_Viewer::run() {
	return GLFW_window::run();
}

World_Viewer::~World_Viewer() {
	delete worldMap;
}

void World_Viewer::keyboard(int key, int scancode, int action, int mods) {
	vec4 rotating_vector = mat4::rotate_x(x_angle_) * mat4::rotate_y(y_angle_) * vec4(0, 0, 1, 0);

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			case GLFW_KEY_W:
				position -= rotating_vector;
				break;

			case GLFW_KEY_S:
				position += rotating_vector;
				break;

			case GLFW_KEY_A:
				position -= mat4::rotate_y(90) * rotating_vector;
				break;

			case GLFW_KEY_D:
				position += mat4::rotate_y(90) * vec4(0, 0, 1, 0);
				break;

			case GLFW_KEY_LEFT:
				y_angle_ += 10.0;
				break;

			case GLFW_KEY_RIGHT:
				y_angle_ -= 10.0;
				break;

			case GLFW_KEY_DOWN:
				x_angle_ -= 10.0;
				break;

			case GLFW_KEY_UP:
				x_angle_ += 10.0;
				break;

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window_, GL_TRUE);
				break;
		}
	}
}
