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


void main()
{
    gl_Position = vec4(v_position.x * 0.5, v_position.y * 0.5, v_position.z * 0.5, 1.0);
}
