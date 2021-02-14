#version 430

layout(location = 0) in vec2 texture_coord;

uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;
layout(location = 1) in vec3 world_position;

void main()
{
	vec3 color = texture(texture_1, texture_coord).xyz;
	if(world_position.y<-1){
	discard;
	}
	out_color = vec4(color, 1);
}