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
	position = worldMap->start_position();
}

void World_Viewer::initialize() {
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	glEnable(GL_DEPTH_TEST);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 center = position;
	vec4 eye = vec4(0, 0, 1, 1.0);
	vec4 up = vec4(0, 1, 0, 0);

	eye = mat4::translate(center) * mat4::rotate_y(yaw) * mat4::rotate_x(pitch_) * eye;
	up = mat4::rotate_y(yaw) * mat4::rotate_x(pitch_) * up;

	auto fovy_ = 45;
	auto near_ = 0.1f;
	auto far_  = 20;

	mat4 view = mat4::look_at(vec3(eye), vec3(center), vec3(up));
	mat4 projection = mat4::perspective(fovy_, (float) width_ / (float) height_, near_, far_);

	// the matrices we need: model
	mat4 m_matrix;

	// the sun is centered at the origin and -- for lighting -- considered to be a point, so that is the light position in world coordinates
	vec4 light = vec4(0.0, 0.0, 0.0, 1.0); //in world coordinates
	light = view * light;

	_shader.use();
	_shader.set_uniform("view_matrix", view);
	_shader.set_uniform("projection_matrix", projection);
	_shader.set_uniform("light_position", light);

	// render cubes
	for (Cube* v : worldMap->_cubes) {
		m_matrix = mat4::translate(v->pos_);
		_shader.set_uniform("model_matrix", m_matrix);
		_shader.set_uniform("u_color", v->color);

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
	vec4 rotating_vector = mat4::rotate_z(pitch_) * mat4::rotate_y(yaw) * vec4(0, 0, 1, 0);

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
				yaw += 10.0;
				break;

			case GLFW_KEY_RIGHT:
				yaw -= 10.0;
				break;

			case GLFW_KEY_DOWN:
				pitch_ -= 10.0;
				break;

			case GLFW_KEY_UP:
				pitch_ += 10.0;
				break;

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window_, GL_TRUE);
				break;
		}
	}

	std::cout << position << " pitch " << pitch_ << " yaw " << yaw << std::endl;
}
