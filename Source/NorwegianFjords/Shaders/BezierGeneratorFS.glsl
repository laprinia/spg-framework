#version 430
layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
layout(location = 3) flat in int isReflective;

layout (binding=0) uniform sampler2D texture_1;
layout (binding=1) uniform sampler2D texture_2;
layout (binding=2) uniform samplerCube texture_cubemap;

uniform vec3 camera_position;
vec3 V;
vec4 reflection()
{
    vec3 reflection=reflect(V, world_normal);

    return texture(texture_cubemap, reflection,1);
}


vec4 refraction( float refractive_index)
{
    vec3 refraction=refract(-V,world_normal,refractive_index);
    return texture(texture_cubemap,refraction,1);
}

float lerp(float a, float b, float w)
{
  return a + w*(b-a);
}

vec4 getColor(vec4 color, vec4 cubeColor)
{
if(isReflective==0){
    return color;
  }

}

void main()
{
 
  vec4 color = texture(texture_1, texture_coord);
  vec4 color2 = texture(texture_2, texture_coord);
  V=normalize(camera_position - world_position);
   
  vec4 cubeColor=mix(refraction(1.0/1.33),reflection(),0.5);
   
   out_color=mix(cubeColor,color,0.1);
   if(isReflective==0){
   out_color=color2;
   }

}