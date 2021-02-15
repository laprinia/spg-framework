#version 430
#define noMountain 0
#define edgeMountain 1
#define inBetweenMountain 2
layout(lines) in;

layout(triangle_strip, max_vertices = 78) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4,control_p5, control_p6, control_p7,
control_p8,control_p9, control_p10, control_p11, control_p12, control_p13;
uniform int instanceNumber;
uniform int pointsNumber;


layout(location = 0 )out vec2 texture_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;
layout(location = 3) flat out int isReflective;

in int instance[2];

vec3[10][2] lastPointsOfLastInstance;

int isIntersecting = 0;

vec3 cubicBezier(float t, vec3 cp1, vec3 cp2, vec3 cp3, vec3 cp4) {
    return cp1 * pow((1 - t), 3) + cp2 * 3 * t * pow((1 - t), 2) + cp3 * 3 * pow(t, 2) * (1 - t) + cp4 * pow(t, 3);
}

void createMountainInstanced(int isFlipped, vec3 startingPoint1, vec3 startingPoint2, float geometryDisplacement1,
                             float heightDisplacement) {

    float instanceDisplacement1 = isFlipped == 1 ? -1 : 1;
    geometryDisplacement1 = isFlipped == 1 ? instance[0] * geometryDisplacement1 : geometryDisplacement1;
    
    gl_Position = Projection * View * vec4(startingPoint1.x + geometryDisplacement1,
                                           instance[0] == 1 ? startingPoint1.y + heightDisplacement : startingPoint1.y,
                                           startingPoint1.z, 1);
    texture_coord = vec2(0, 0);
    world_normal = normalize(startingPoint1);
    world_position = startingPoint1;
    isReflective = 0;
    EmitVertex();

    vec3 newPoint1 = vec3(startingPoint1.x + geometryDisplacement1 + instanceDisplacement1,
                          instance[0] == 0 ? startingPoint1.y + heightDisplacement : startingPoint1.y,
                          startingPoint1.z);

    gl_Position = Projection * View * vec4(newPoint1, 1);
    texture_coord = vec2(1, 0);
    world_normal = normalize(newPoint1);
    world_position = newPoint1;
    isReflective = 0;
    EmitVertex();

    gl_Position = Projection * View * vec4(startingPoint2.x + geometryDisplacement1,
                                           instance[0] == 1 ? startingPoint2.y + heightDisplacement : startingPoint2.y,
                                           startingPoint2.z, 1);
    texture_coord = vec2(0, 1);
    world_normal = normalize(startingPoint2);
    world_position = startingPoint2;
    isReflective = 0;
    EmitVertex();

    vec3 newPoint2 = vec3(startingPoint2.x + geometryDisplacement1 + instanceDisplacement1,
                          instance[0] == 0 ? startingPoint2.y + heightDisplacement : startingPoint2.y,
                          startingPoint2.z);

    gl_Position = Projection * View * vec4(newPoint2, 1);
    texture_coord = vec2(1, 1);
    world_normal = normalize(newPoint2);
    world_position = newPoint2;
    isReflective = 0;
    EmitVertex();


    EndPrimitive();

}

vec3 getLineIntersect(float p0_x, float p0_y, float p1_x, float p1_y,
                      float p2_x, float p2_y, float p3_x, float p3_y) {
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        float intersectX = p0_x + (t * s1_x);
        float intersectY = p0_y + (t * s1_y);
        return vec3(intersectX, intersectY, 0);
    }

    return vec3(0, 0, 0);
}

void createRiverBranch(vec3 cp1, vec3 cp2, vec3 cp3, vec3 cp4,int leftMountainType,int rightMountainType)
{
vec3 offset = vec3(1, 0, 0) * instance[0];
vec3 offset2 = vec3(1, 0, 0) * (instance[0]);
vec3 offset3 = vec3(1, 0, 0) * (instance[0] + 1);
vec3 offset4 = vec3(1, 0, 0) * (instance[0] + 1);

for (float t = 0.0; t < 1; t += 1.0 / pointsNumber) {

vec3 p1 = cubicBezier(t, cp1, cp2, cp3, cp4);
vec3 p2 = cubicBezier(t + 1.0 / pointsNumber, cp1, cp2, cp3, cp4);
vec3 p3 = cubicBezier(t, cp1, cp2, cp3, cp4);
vec3 p4 = cubicBezier(t + 1.0 / pointsNumber, cp1, cp2, cp3, cp4);

gl_Position = Projection * View * vec4(p1 + offset, 1);
texture_coord = vec2(0, 0);
world_normal = normalize(p1 + offset);
world_position = p1 + offset;
isReflective = 1;

EmitVertex();

gl_Position = Projection * View * vec4(p3 + offset3, 1);
texture_coord = vec2(1, 0);
world_normal = normalize(p3 + offset3);
world_position = p3 + offset3;
isReflective = 1;

EmitVertex();


gl_Position = Projection * View * vec4(p2 + offset2, 1);
texture_coord = vec2(0, 1);
world_normal = normalize(p2 + offset2);
world_position = p2 + offset2;
isReflective = 1;

EmitVertex();

gl_Position = Projection * View * vec4(p4 + offset4, 1);
texture_coord = vec2(1, 1);
world_normal = normalize(p4 + offset4);
world_position = p4 + offset4;
isReflective = 1;

EmitVertex();


EndPrimitive();

//right mountain
switch(rightMountainType){
case edgeMountain: createMountainInstanced(0,p3+offset3, p4+offset4,2,3); break;
case inBetweenMountain:
break;
}

//left mountain
switch(leftMountainType){
case edgeMountain:
createMountainInstanced(1,p1+offset, p2+offset2,-2,3);
break;
case inBetweenMountain:

//todo

break;
}
if(instance[0]==2){

lastPointsOfLastInstance[pointsNumber][0]=p3+offset3;
lastPointsOfLastInstance[pointsNumber][1]=p4+offset4;
}
}
}


void main() {


    createRiverBranch(control_p1, control_p2, control_p3, control_p4, 1, 1);

    //la stanga
    createRiverBranch(control_p4, control_p11, control_p12, control_p13, 1, 0);


    //centru
     createRiverBranch(control_p4, control_p5, control_p6, control_p7,2,0);


    //la dreapta
     createRiverBranch(control_p4, control_p8, control_p9, control_p10,2,1);
}