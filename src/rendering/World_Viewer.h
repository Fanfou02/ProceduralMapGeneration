//
// Created by zyuiop on 18/05/19.
//

#ifndef RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
#define RANDOMTILEMAPGENERATION_WORLD_VIEWER_H


#include <utils.h>
#include <shader.h>
#include <bezier.h>
#include "glfw_window.h"
#include "voxels.h"
#include "cube.h"
#include "World_Map.h"

class World_Viewer : public GLFW_window {

	void initialize() override;

	void resize(int width, int height) override;

	void paint() override;
	void gen_shadows();

protected:
	void keyboard(int key, int scancode, int action, int mods) override;

	void timer(float diff_sec) override;

public:
	World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels, int groundLevel, int groupSize = 10);

	~World_Viewer() override;
	int run() override;


private:
	World_Map *worldMap;
	vec4 position = vec4(0, 0, 0, 1.0);
	Shader _shader;
	Shader _depth_shader;

	PiecewiseBezier _pos_bezier;
	PiecewiseBezier _view_angle_bezier;

	float bezier_speed = 1.0/30.0f; // 1 = curve in 1 second
	float bezier_current = 0;

	bool bezier_start = false;
	bool paused = true;

	/// current viewport dimension
	int  width_, height_;

	float yaw = 45;
	float pitch_ = -45;


	mat4 gen_view(vec3 const& center, float const& yaw, float const& pitch_);
	mat4 gen_projection(float const& width, float const& height);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO;
	unsigned int depthMap;
};


#endif //RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
