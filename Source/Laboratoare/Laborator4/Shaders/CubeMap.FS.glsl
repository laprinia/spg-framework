#version 430

layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;

uniform vec3 camera_position;

layout(location = 0) out vec4 out_color;

vec3 V;
vec3 myReflect()
{
    vec3 reflection=reflect(-V, world_normal);

    return texture(texture_cubemap, reflection).xyz;
}


vec3 myRefract(float refractive_index)
{
    vec3 refraction=refract(-V,world_normal,refractive_index);
    return texture(texture_cubemap,refraction).xyz;
}



void main()
{
    V=normalize(camera_position-world_position);
    out_color = vec4(myReflect(), 0);
	out_color += vec4(myRefract(1.0/1.33), 0);
}
