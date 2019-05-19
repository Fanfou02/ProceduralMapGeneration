#include <utility>

//
// Created by zyuiop on 18/05/19.
//

#include "World_Viewer.h"
#include "voxels.h"

World_Viewer::World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels) : GLFW_window(_title,
																												 _width,
																												 _height) {
	_cubes = std::vector<Cube*>();

	for (auto voxel : voxels) {
		_cubes.push_back(new Cube(voxel.x, voxel.y, voxel.z, voxel.color));
	}
}

void GLAPIENTRY MessageCallback(GLenum source,
								GLenum type,
								GLuint id,
								GLenum severity,
								GLsizei length,
								const GLchar *message,
								const void *userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
}



void World_Viewer::initialize() {
// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

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

	vec4 center = vec4(0, 0, 0, 0);
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

	// render cubes
	for (Cube* v : _cubes) {
		m_matrix = mat4::translate(v->pos_);
		mv_matrix = view * m_matrix;
		mvp_matrix = projection * mv_matrix * mat4::scale(0.01f);

		_shader.use();
		_shader.set_uniform("modelview_projection_matrix", mvp_matrix);
		_shader.set_uniform("color", vec4(1.0f, 0.5f, 0.2f, 1.0f));

		v->draw();
	}

	// check for OpenGL errors
	glCheckError();
}

int World_Viewer::run() {
	return GLFW_window::run();
}

World_Viewer::~World_Viewer() {
	for (Cube* v : _cubes) {
		delete v;
	}
}
