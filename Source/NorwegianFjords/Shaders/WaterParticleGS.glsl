#version 430
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;

layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 world_position;

vec3 vpos = gl_in[0].gl_Position.xyz;
vec3 forward = normalize(eye_position - vpos);
vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
vec3 up = normalize(cross(forward, right));

void EmitPoint(vec2 offset)
{
	vec3 pos = right * offset.x + up * offset.y + vpos;
	gl_Position = Projection * View * vec4(pos, 1.0);
	EmitVertex();
}

void main()
{
	float ds = 0.01f;
	
	texture_coord = vec2(0, 0);
	world_position=vpos;
	EmitPoint(vec2(-ds,-ds));
	texture_coord = vec2(1, 0);
	world_position=vpos;
	EmitPoint(vec2(ds,-ds));
	texture_coord = vec2(0, 1);
	world_position=vpos;
	EmitPoint(vec2(-ds,ds));
	texture_coord = vec2(1, 1);
	world_position=vpos;
	EmitPoint(vec2(ds,ds));

	EndPrimitive();


}
