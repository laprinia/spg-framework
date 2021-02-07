#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;

out int instance;

void main()
{
	instance = gl_InstanceID;
	gl_Position =  Model * vec4(v_position, 1);
}

