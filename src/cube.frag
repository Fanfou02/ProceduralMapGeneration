//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 330 core

in vec3 v2f_normal;
in vec3 v2f_light;
in vec3 v2f_view;

uniform vec3 u_color;
out vec4 f_color;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main() {
    /**
   *  Implement the Phong shading model (like in the 1st exercise) by using the passed
   *  variables and write the resulting color to `color`.
   *  `tex` should be used as material parameter for ambient, diffuse and specular lighting.
   * Hints:
   * - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use
   * `texture(...).r` to get just the red component or `texture(...).rgb` to get a vec3 color
   * value
    */



    vec3 color = vec3(0.0,0.0,0.0);
    vec3 material = u_color;
/*
    // compute the ambient lightning
    vec3 ambient = 0.2 * sunlight * material;
    color += ambient;

    // compute the diffuse lightning
    float v = dot(v2f_normal, v2f_light);
    if (v > 0)
        color += sunlight * material * v;

    // compute the specular lightning
    vec3 reflected_ray = reflect(v2f_light, v2f_normal);
    float t = dot(reflected_ray, v2f_view);
    if (t > 0 && v > 0)
        color += sunlight * material * pow(t, shininess);

    // add required alpha value
    f_color = vec4(color, 1.0);*/
    f_color = vec4(u_color, 1.0);
}
