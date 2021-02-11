#version 430
layout(lines) in;

layout(triangle_strip, max_vertices = 78) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4,control_p5, control_p6, control_p7, control_p8,control_p9, control_p10, control_p11, control_p12, control_p13;
uniform int instanceNumber;
uniform int pointsNumber;


layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;
layout(location = 3) flat out int isReflective;
in int instance[2];



vec3 cubicBezier(float t,vec3 cp1,vec3 cp2,vec3 cp3, vec3 cp4)
{
	return cp1 * pow((1 - t), 3) + cp2 * 3 * t * pow((1 - t), 2) + cp3 * 3 * pow(t, 2) * (1 - t) + cp4 * pow(t, 3);
}

void createRiverBranch(vec3 cp1,vec3 cp2, vec3 cp3,vec3 cp4)
{
    vec3 offset = vec3(1, 0, 0) * instance[0];
    vec3 offset2 = vec3(1, 0, 0) * (instance[0]);
    vec3 offset3 = vec3(1, 0, 0) * (instance[0] + 1);
    vec3 offset4 = vec3(1, 0, 0) * (instance[0] + 1);

    for (float t = 0.0; t < 1; t += 1.0 / pointsNumber) {

        vec3 p1 = cubicBezier(t,cp1, cp2, cp3, cp4);
        vec3 p2 = cubicBezier(t + 1.0 / pointsNumber,cp1, cp2, cp3, cp4);
        vec3 p3 = cubicBezier(t,cp1, cp2, cp3, cp4);
        vec3 p4 = cubicBezier(t + 1.0 / pointsNumber,cp1, cp2, cp3, cp4);

        gl_Position = Projection * View * vec4(p1 + offset, 1);
        texture_coord = vec2(0, 0);
        world_normal = normalize(p1+offset);
        world_position=p1 + offset;
        isReflective=1;
        EmitVertex();

        gl_Position = Projection * View * vec4(p3 + offset3, 1);
        texture_coord = vec2(1, 0);
        world_normal = normalize(p3+offset3);
        world_position=p3 + offset3;
        isReflective=1;
        EmitVertex();


        gl_Position = Projection * View * vec4(p2 + offset2, 1);
        texture_coord = vec2(0, 1);
        world_normal = normalize(p2+offset2);
        world_position=p2 + offset2;
        isReflective=1;
        EmitVertex();

        gl_Position = Projection * View * vec4(p4 + offset4, 1);
        texture_coord = vec2(1, 1);
        world_normal = normalize(p4+offset4);
        world_position=p4 + offset4;
        isReflective=1;
        EmitVertex();

        //right mountain
        if(instance[0]==2){

        gl_Position = Projection * View * vec4(p3 + offset3, 1);
        texture_coord = vec2(0, 0);
        world_normal = normalize(p3+offset3);
        world_position=p3 + offset3;
        isReflective=0;
        EmitVertex();

        vec3 newp3=vec3(p3.x+offset3.x+5.0, p3.y+offset3.y+2, p3.z+offset3.z);

        gl_Position = Projection * View * vec4(newp3,1);
        texture_coord = vec2(1, 0);
        world_normal = normalize(newp3);
        world_position=newp3;
        isReflective=0;
        EmitVertex();

        gl_Position = Projection * View * vec4(p4 + offset4, 1);
        texture_coord = vec2(0, 1);
        world_normal = normalize(p4+offset4);
        world_position=p4 + offset4;
        isReflective=0;
        EmitVertex();
       
        vec3 newp4=vec3(p4.x+offset4.x+5.0, p4.y+offset.y+2, p4.z+offset.z);

        gl_Position = Projection * View * vec4(newp4,1);
        texture_coord = vec2(1,1);
        world_normal = normalize(newp4);
        world_position=newp4;
        isReflective=0;
        EmitVertex();


        }

        EndPrimitive();
    }
    
}
 void createMountains() {


 }

void main() {
   

    createRiverBranch(control_p1, control_p2, control_p3, control_p4);
    //createRiverBranch(control_p4, control_p5, control_p6, control_p7);
   //createRiverBranch(control_p4, control_p8, control_p9, control_p10);
    //createRiverBranch(control_p4, control_p11, control_p12, control_p13);
    
    
}