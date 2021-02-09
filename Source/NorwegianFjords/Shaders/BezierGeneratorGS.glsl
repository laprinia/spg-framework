#version 430
layout(lines) in;

layout(triangle_strip, max_vertices = 80) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4;
uniform int instanceNumber;
uniform int pointsNumber;

//layout(location = 1) in vec3 world_position;
//layout(location = 2) in vec3 world_normal;

layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;

in int instance[2];


vec3 translateX(vec3 point, float t)
{
	return vec3(point.x + t, point.y, point.z);
}

vec3 cubicBezier(float t)
{
	return control_p1 * pow((1 - t), 3) + control_p2 * 3 * t * pow((1 - t), 2) + control_p3 * 3 * pow(t, 2) * (1 - t) + control_p4 * pow(t, 3);
}

void main() {


    vec3 offset = vec3(1, 0, 0) * instance[0];
    vec3 offset2 = vec3(1, 0, 0) * (instance[0]);
    vec3 offset3 = vec3(1, 0, 0) * (instance[0] + 1);
    vec3 offset4 = vec3(1, 0, 0) * (instance[0] + 1);

    for (float t = 0.0; t < 1; t += 1.0 / pointsNumber) {

        vec3 p1 = cubicBezier(t);
        vec3 p2 = cubicBezier(t + 1.0 / pointsNumber);
        vec3 p3 = cubicBezier(t);
        vec3 p4 = cubicBezier(t + 1.0 / pointsNumber);

        gl_Position = Projection * View * vec4(p1 + offset, 1);
        texture_coord = vec2(0, 0);
        world_normal = normalize(p1+offset);
        world_position=p1 + offset;
        EmitVertex();

        gl_Position = Projection * View * vec4(p3 + offset3, 1);
        texture_coord = vec2(1, 0);
        world_normal = normalize(p3+offset3);
        world_position=p3 + offset3;
        EmitVertex();


        gl_Position = Projection * View * vec4(p2 + offset2, 1);
        texture_coord = vec2(0, 1);
        world_normal = normalize(p2+offset2);
        world_position=p2 + offset2;
        EmitVertex();

        gl_Position = Projection * View * vec4(p4 + offset4, 1);
        texture_coord = vec2(1, 1);
        world_normal = normalize(p4+offset4);
        world_position=p4 + offset4;
        EmitVertex();
        
    }
    EndPrimitive();
}