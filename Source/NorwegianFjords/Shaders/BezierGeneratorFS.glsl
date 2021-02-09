#version 430
layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
uniform sampler2D texture_1;


uniform vec3 camera_position;
uniform samplerCube texture_cubemap;

vec3 V;
vec3 reflection()
{
    vec3 reflection=reflect(V, world_normal);

    return texture(texture_cubemap, reflection).xyz;
}


vec3 refraction( float refractive_index)
{
    vec3 refraction=refract(-V,world_normal,refractive_index);
    return texture(texture_cubemap,refraction).xyz;
}

float lerp(float a, float b, float w)
{
  return a + w*(b-a);
}

void main()
{
 
   vec3 color = texture(texture_1, texture_coord).xyz;
    
   V=normalize(camera_position - world_position);
   
   vec3 cubeColor=mix(refraction(1.0/1.33),reflection(),0.5);
   vec3 finalColor=mix(color,cubeColor,0.5);

   out_color = vec4(finalColor,1);
}