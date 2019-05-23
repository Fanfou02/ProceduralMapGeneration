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
layout (location = 2) in vec3 v_color;

uniform vec3 light_direction; //in eye space coordinates already
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;

out vec3 v2f_normal;
out vec3 v2f_light;
out vec3 v2f_view;
out vec3 v2f_color;

void main()
{
    vec4 position = vec4(v_position.xyz, 1.0);
    mat4 modelview_matrix = view_matrix * model_matrix;
    vec4 vertices = (modelview_matrix * position);

    mat3 normal_matrix = mat3(transpose(inverse(modelview_matrix)));

    v2f_normal = normalize(normal_matrix * v_normal);
    v2f_light = normalize(- normal_matrix * light_direction);
    v2f_view = normalize(vertices.xyz - v2f_light);
    v2f_color = v_color;

    gl_Position = (projection_matrix * modelview_matrix) * position;
}
