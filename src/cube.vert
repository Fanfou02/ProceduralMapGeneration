//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;

uniform vec4 light_position; //in eye space coordinates already
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 other_uniform;

out vec3 v2f_normal;
out vec3 v2f_light;
out vec3 v2f_view;


void main()
{
    vec4 position = vec4(v_position.x, v_position.y, v_position.z, 1.0);
    mat4 modelview_matrix = view_matrix * model_matrix;
    vec4 vertices = (modelview_matrix * position);

    mat3 normal_matrix = transpose(inverse(mat3(modelview_matrix)));

    v2f_normal = normalize(normal_matrix * v_normal);
    v2f_light = normalize(light_position.xyz - vertices.xyz);
    v2f_view = normalize(vertices.xyz - v2f_light);

    gl_Position = (projection_matrix * modelview_matrix) * position;
}
