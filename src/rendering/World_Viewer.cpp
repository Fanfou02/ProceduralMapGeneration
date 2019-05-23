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
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_shader.load(SHADER_PATH   "/cube.vert", SHADER_PATH   "/cube.frag");

	// Setup shadow map // https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void World_Viewer::resize(int width, int height) {
	width_ = width;
	height_ = height;
	glViewport(0, 0, width, height);

	std::cout << "Resize " << width << " " << height << std::endl;
}


void World_Viewer::gen_shadows() {
	// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

	vec4 center = vec4(300 * vec3(0.4f, 1.0f, 0.6f), 1.0);
	vec4 eye = vec4(0, 0, 1, 1.0);
	vec4 up = vec4(0, 1, 0, 0);

	float yaw = 40, pitch_ = -65;

	eye = mat4::translate(center) * mat4::rotate_y(yaw) * mat4::rotate_x(pitch_) * eye;
	up = mat4::rotate_y(yaw) * mat4::rotate_x(pitch_) * up;


	auto fovy_ = 45;
	auto near_ = 0.1f;
	auto far_  = 20;

	mat4 view = mat4::look_at(vec3(eye), vec3(center), vec3(up));
	mat4 projection = mat4::perspective(fovy_, (float) SHADOW_WIDTH / (float) SHADOW_HEIGHT, near_, far_);
	mat4 lightSpaceMatrix = projection * view;

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	// ConfigureShaderAndMatrices();
	// RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void World_Viewer::paint() {
	this->gen_shadows();

	glViewport(0, 0, width_, height_);
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
	mat4 viewprojection_matrix = projection * view;

	_shader.use();
	_shader.set_uniform("light_direction", vec3(-0.4f, -1.0f, -0.6f));
	_shader.set_uniform("view_matrix", view);
	_shader.set_uniform("viewprojection_matrix", viewprojection_matrix);

	glBindTexture(GL_TEXTURE_2D, depthMap);

	worldMap->draw();


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
	mat4 rot = mat4::rotate_y(yaw);

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			case GLFW_KEY_W:
				position -= rot * vec4(0, 0, 1, 0);
				break;

			case GLFW_KEY_S:
				position += rot * vec4(0, 0, 1, 0);
				break;

			case GLFW_KEY_A:
				position -=  rot * vec4(1, 0, 0, 0);
				break;

			case GLFW_KEY_D:
				position +=  rot * vec4(1, 0, 0, 0);
				break;

			case GLFW_KEY_SPACE:
			case GLFW_KEY_E:
				position += vec4(0, 1, 0, 0);
				break;

			case GLFW_KEY_LEFT_SHIFT:
			case GLFW_KEY_Q:
				position -= vec4(0, 1, 0, 0);
				break;

			case GLFW_KEY_LEFT:
				yaw += 5.0;
				break;

			case GLFW_KEY_RIGHT:
				yaw -= 5.0;
				break;

			case GLFW_KEY_DOWN:
				if (pitch_ > -85) pitch_ -= 5.0;
				break;

			case GLFW_KEY_UP:
				if (pitch_ < 85) pitch_ += 5.0;
				break;

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window_, GL_TRUE);
				break;
		}
	}
}
