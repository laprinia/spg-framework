#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

//layout(location = 1) out vec3 world_position;
//layout(location = 2) out vec3 world_normal;

out int instance;
out int isReflectiveRiver;
void main()
{
	instance = gl_InstanceID;
	isReflectiveRiver=1;
    gl_Position = Model * vec4(v_position.x,v_position.y,v_position.z, 1);
}


