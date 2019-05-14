//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /** \todo
    * - Copy your working code from the fragment shader of your Phong shader use it as
    * starting point
    * - instead of using a single texture, use the four texures `day_texure`, `night_texure`,
    * `cloud_texure` and `gloss_texture` and mix them for enhanced effects
    * Hints:
    * - cloud and gloss textures are just greyscales. So you'll just need one color-
    * component.
    * - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use
    * `texture(...).r` to get just the red component or `texture(...).rgb` to get a vec3 color
    * value
    * - use mix(vec3 a,vec3 b, s) = a*(1-s) + b*s for linear interpolation of two colors
     */

    // Init the day, night and cloud colors 
    vec3 day_color = vec3(0.0,0.0,0.0);
    vec3 night_color = vec3(0.0,0.0,0.0);
    vec3 cloud_color = vec3(0.0,0.0,0.0);

    // Fetch the textures
    vec3 day_texture = texture(day_texture, v2f_texcoord.st).rgb;
    vec3 night_texture = texture(night_texture, v2f_texcoord.st).rgb;
    float cloud_texture = texture(cloud_texture, v2f_texcoord.st).r;
    float gloss_texture = texture(gloss_texture, v2f_texcoord.st).r;
    
    // compute the ambient lightning
    day_color += 0.2 * sunlight * day_texture;
    cloud_color += 0.2 * sunlight * cloud_texture;
    
    // compute the diffuse lightning
    float v = dot(v2f_normal, v2f_light);
    if (v > 0){
        day_color += sunlight * day_texture * v;
        cloud_color += sunlight * cloud_texture * v;
    }
    
    // compute the specular lightning
    vec3 reflected_ray = reflect(v2f_light, v2f_normal);
    float t = dot(reflected_ray, v2f_view);
    vec3 specular_material = vec3(1, 1, 1);
    if (t > 0 && v > 0){
        // Compute the specularity value
        float specular_amount = gloss_texture * (1.0 - cloud_texture);
        // modulate the specular component by the specularity value
        specular_material *= specular_amount;
        day_color += sunlight * specular_material * pow(t, shininess);
    }

    // Compute day color by mixing it with the clouds
    day_color = mix(day_color, cloud_color, cloud_texture);

    // Compute night color
    night_color = night_texture * (1.0 - cloud_texture);

    // Mix day and night colors based on the amount of sunlight
    vec3 color = mix(night_color, day_color, v);

    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color, 1.0);

}
