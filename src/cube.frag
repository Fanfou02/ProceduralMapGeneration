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
in vec3 v2f_color;
in vec4 v2f_lightpos;

out vec4 f_color;

uniform sampler2D shadow_map;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);
const float ambient_value = 0.4;
const float shadow_bias = 0.000002;
const int average_size = 1; // number of blocks to use in each direction (0 = no PCF)

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
    vec3 material = v2f_color;

    // compute the ambient lightning
    vec3 ambient = ambient_value * sunlight * material;
    color += ambient;

    // compute the diffuse lightning
    float v = dot(v2f_normal, v2f_light);
    vec3 diffuse = vec3(0, 0, 0);
    if (v > 0)
        diffuse = sunlight * material * v;
    color += diffuse;

    // compute the specular lightning
    vec3 reflected_ray = reflect(v2f_light, v2f_normal);
    float t = dot(reflected_ray, v2f_view);
    vec3 specular = vec3(0, 0, 0);
    if (t > 0 && v > 0)
        specular = sunlight * material * pow(t, shininess);
    color += specular;

    // compute shadows
    float shadow = 0.0;
    vec2 texture_pixel_size = 1.0 / textureSize(shadow_map, 0);
    vec3 lightpos_proj = (v2f_lightpos.xyz / v2f_lightpos.w) * 0.5 + 0.5;
    float point_depth = lightpos_proj.z;

    // Compute the average
    for (int x = -average_size; x <= average_size; ++x) {
        for (int y = -average_size; y <= average_size; ++y) {
            float pcf_depth = texture(shadow_map, lightpos_proj.xy + vec2(x, y) * texture_pixel_size).r;
            shadow += point_depth - shadow_bias > pcf_depth ? 1.0 : 0.0;
        }
    }

    shadow /= pow(float(average_size) * 2.0 + 1.0, 2);
    // add required alpha value
    f_color = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) * material, 1.0);
}
