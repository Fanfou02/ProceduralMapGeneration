/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#include <utility>
#include "World_Viewer.h"
#include "voxels.h"
#include "math.h"

World_Viewer::World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> &voxels, int groundLevel,
						   int groupSize) : GLFW_window(_title,
														_width,
														_height) {
	worldMap = new World_Map(voxels, groundLevel, groupSize);
	_pos_bezier = PiecewiseBezier();
	_view_angle_bezier = PiecewiseBezier();

	std::vector<vec3> control;
	control.emplace_back(worldMap->min_x, 80, worldMap->min_z);
	control.emplace_back(worldMap->max_x, 40, worldMap->min_z);
	control.emplace_back(worldMap->max_x, 10, worldMap->max_z);
	control.emplace_back(worldMap->min_x, 40, worldMap->max_z);
	control.emplace_back(worldMap->min_x, 80, worldMap->min_z);

	std::vector<vec3> view_angle_control;
	view_angle_control.emplace_back(60, 25, 60);

	_pos_bezier.set_control_polygon(control);
	_view_angle_bezier.set_control_polygon(view_angle_control);

	position = worldMap->start_position();
}

void World_Viewer::initialize() {
	// During init, enable debug output
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_shader.load(SHADER_PATH   "/cube.vert", SHADER_PATH   "/cube.frag");
	_depth_shader.load(SHADER_PATH   "/depth.vert", SHADER_PATH   "/depth.frag");

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

mat4 World_Viewer::gen_view(vec3 const &center, float const &yaw, float const &pitch) {
	vec4 eye = vec4(0, 0, 1, 1.0);
	vec4 up = vec4(0, 1, 0, 0);

	eye = mat4::translate(center) * mat4::rotate_y(yaw) * mat4::rotate_x(pitch) * eye;
	up = mat4::rotate_y(yaw) * mat4::rotate_x(pitch) * up;

	return mat4::look_at(vec3(eye), vec3(center), vec3(up));
}

mat4 World_Viewer::gen_projection(float const &width, float const &height) {
	auto fovy_ = 45;
	auto near_ = 0.1f;
	auto far_ = 20;

	return mat4::perspective(fovy_, width / height, near_, far_);
}


void World_Viewer::paint() {
	vec3 light_position = 300 * vec3(0.4f, 1.0f, 0.6f);
	mat4 main_m_matrix = mat4::translate(vec4(0, 0, 0, 0));

	// Generate shadows
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	vec4 lightCenter = vec4(light_position, 1.0);
	mat4 lightView = gen_view(lightCenter, 40, -65);
	mat4 lightProjection = gen_projection(SHADOW_WIDTH, SHADOW_HEIGHT);
	mat4 lightSpaceMatrix = lightProjection * lightView;

	_depth_shader.use();
	_depth_shader.set_uniform("lightspace_matrix", lightSpaceMatrix);
	_depth_shader.set_uniform("model_matrix", main_m_matrix);

	glCullFace(GL_FRONT);
	worldMap->draw(); // Draw the shadows
	glCullFace(GL_BACK);


	// Generate image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width_, height_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 center = position;

	mat4 view = gen_view(center, yaw, pitch_);
	mat4 projection = gen_projection(width_, height_);

	_shader.use();
	_shader.set_uniform("light_position", light_position);
	_shader.set_uniform("lightspace_matrix", lightSpaceMatrix);


	mat4 modelview_matrix = view * main_m_matrix;
	mat4 viewprojection_matrix = projection * modelview_matrix;
	_shader.set_uniform("model_matrix", main_m_matrix);
	_shader.set_uniform("modelview_matrix", modelview_matrix);
	_shader.set_uniform("modelviewprojection_matrix", viewprojection_matrix);

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

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_W:
				position -= rot * vec4(0, 0, 1, 0);
				break;

			case GLFW_KEY_S:
				position += rot * vec4(0, 0, 1, 0);
				break;

			case GLFW_KEY_A:
				position -= rot * vec4(1, 0, 0, 0);
				break;

			case GLFW_KEY_D:
				position += rot * vec4(1, 0, 0, 0);
				break;

			case GLFW_KEY_E:
				position += vec4(0, 1, 0, 0);
				break;

			case GLFW_KEY_Q:
				position -= vec4(0, 1, 0, 0);
				break;

			case GLFW_KEY_F:
				speedup_ *= 0.95f;
				std::cout << speedup_ << std::endl;
				break;

			case GLFW_KEY_R:
				speedup_ /= 0.95f;
				std::cout << speedup_ << std::endl;
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

			case GLFW_KEY_SPACE:
				paused = !paused;
				break;

			case GLFW_KEY_B:
				bezier_start = !bezier_start;
				break;

			case GLFW_KEY_ENTER:
				worldMap->render_all_now();
				break;
		}
	}
}

void World_Viewer::timer(float diff_sec) {
	GLFW_window::timer(diff_sec);

	if (!paused)
		worldMap->timer(diff_sec * speedup_);

	if (bezier_start) {
		position = vec4(_pos_bezier(bezier_current), 1.0f);

		// Compute angle
		vec3 cont_position = _view_angle_bezier(bezier_current);

		float dX = position.x - cont_position.x;
		float dY = position.y - cont_position.y;
		float dZ = position.z - cont_position.z;

		yaw = atan2(dX, dZ) * (180.0 / M_PI); // OK
		pitch_ = (atan2(sqrt(dZ * dZ + dX * dX), dY) - M_PI / 2) * (180.0 / M_PI);
		if (pitch_ > 2 * M_PI) pitch_ -= 2 * M_PI;

		bezier_current += bezier_speed * diff_sec;


		while (bezier_current > 1.0f) bezier_current -= 1.0f;
	}
}
