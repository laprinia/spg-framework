#version 430
layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
uniform sampler2D texture_1;


uniform vec3 camera_position;
uniform samplerCube texture_cubemap;

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

void main()
{
 
   vec4 color = texture(texture_1, texture_coord);
    
   V=normalize(camera_position - world_position);
   
   vec4 cubeColor=mix(refraction(1.0/1.33),reflection(),0.5);
 
  // vec4 actualColor=vec4(color.r,color.g,color.b,1);
   vec4 newNew=vec4(lerp(color.r,cubeColor.r,0.1),lerp(color.g,cubeColor.g,0.1),lerp(color.b,cubeColor.b,0.1),1);
  vec4 other=vec4(newNew.x,newNew.y,newNew.z, 1);
   out_color = cubeColor;

}