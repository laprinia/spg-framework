#version 430
layout(lines) in;

layout(line_strip, max_vertices = 256) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4;
uniform int instanceNumber;
uniform int pointsNumber;


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
        EmitVertex();

        gl_Position = Projection * View * vec4(p2 + offset2, 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(p3 + offset3, 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(p3 + offset3, 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(p4 + offset4, 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(p2 + offset2, 1);
        EmitVertex();
        
    }
    EndPrimitive();
}