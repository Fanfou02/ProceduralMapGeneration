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

uniform sampler2D shadowMap;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.000001;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

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
    vec3 ambient = 0.2 * sunlight * material;
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

    float shadow = ShadowCalculation(v2f_lightpos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * material;

    // add required alpha value
    f_color = vec4(lighting, 1.0);
}
