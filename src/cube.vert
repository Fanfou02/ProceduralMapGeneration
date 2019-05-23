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

uniform vec3 light_position; //in eye space coordinates already
uniform mat4 view_matrix;
uniform mat4 viewprojection_matrix;
uniform mat4 lightSpaceMatrix;

out vec3 v2f_normal;
out vec3 v2f_light;
out vec3 v2f_view;
out vec3 v2f_color;
out vec4 v2f_lightpos;

void main()
{
    vec4 position = vec4(v_position.xyz, 1.0);
    vec4 vertices = (view_matrix * position);

    v2f_normal = normalize(v_normal);
    v2f_light = normalize(light_position - vertices.xyz);
    v2f_view = normalize(vertices.xyz - v2f_light);
    v2f_color = v_color;
    v2f_lightpos = lightSpaceMatrix * vec4(v_position, 1.0);

    gl_Position = viewprojection_matrix * position;
}
