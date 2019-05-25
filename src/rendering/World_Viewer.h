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
	void gen_shadows();

protected:
	void keyboard(int key, int scancode, int action, int mods) override;

	void timer() override;

public:
	World_Viewer(const char *_title, int _width, int _height, std::vector<Voxel> voxels);

	~World_Viewer() override;
	int run() override;


private:
	World_Map *worldMap;
	vec4 position = vec4(0, 0, 0, 1.0);
	Shader _shader;
	Shader _depth_shader;

	/// current viewport dimension
	int  width_, height_;

	/// rotation in x direction around the planet/sun from the original point
	float pitch_ = -45;
	/// rotation in y direction around the planet/sun from the original point
	float yaw = 45;

	mat4 gen_view(vec3 const& center, float const& yaw, float const& pitch_);
	mat4 gen_projection(float const& width, float const& height);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO;
	unsigned int depthMap;
};


#endif //RANDOMTILEMAPGENERATION_WORLD_VIEWER_H
