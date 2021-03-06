#version 430
layout(lines) in;
//TODO 
//prima data generati o curba (cu line strip), apoi o suprafata de rotatie/translatie cu triangle_strip
layout(triangle_strip, max_vertices = 256) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4;
uniform int no_of_instances;
uniform int no_of_generated_points;


in int instance[2];


vec3 rotateY(vec3 point, float u)
{
	float x = point.x * cos(u) - point.z *sin(u);
	float z = point.x * sin(u) + point.z *cos(u);
	return vec3(x, point.y, z);
}

vec3 translateX(vec3 point, float t)
{
	return vec3(point.x + t, point.y, point.z);
}

vec3 bezier(float t)
{
	return control_p1 * pow((1 - t), 3) + control_p2 * 3 * t * pow((1 - t), 2) + control_p3 * 3 * pow(t, 2) * (1 - t) + control_p4 * pow(t, 3);
}

void main()
{

    vec3 offset = vec3(1, 0, 0) * instance[0];
    vec3 offset2 = vec3(1, 0, 0) * (instance[0]);
    vec3 offset3 = vec3(1, 0, 0) * (instance[0] + 1);
    vec3 offset4 = vec3(1, 0, 0) * (instance[0] + 1);
    offset2 = rotateY(offset2, 0.09);
    offset4 = rotateY(offset4, 0.09);

    for (float t = 0.0; t < 1; t += 1.0 / no_of_generated_points) {
        offset = rotateY(offset, 0.09);
        offset2 = rotateY(offset2, 0.09);
        offset3 = rotateY(offset3, 0.09);
        offset4 = rotateY(offset4, 0.09);
        vec3 p1 = bezier(t);
        vec3 p2 = bezier(t + 1.0 / no_of_generated_points);
        vec3 p3 = bezier(t);
        vec3 p4 = bezier(t + 1.0 / no_of_generated_points);

        gl_Position = Projection* View * vec4(p1 + offset, 1);	EmitVertex();
        gl_Position = Projection * View * vec4(p3 + offset3, 1);	EmitVertex();
        gl_Position = Projection * View * vec4(p2 + offset2, 1);	EmitVertex();
        gl_Position = Projection * View * vec4(p4 + offset4, 1);	EmitVertex();
    }
    EndPrimitive();
}