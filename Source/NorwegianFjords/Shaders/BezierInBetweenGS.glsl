#version 430
layout(lines) in;

layout(triangle_strip, max_vertices = 78) out;

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4,control_p5, control_p6, control_p7,
control_p8,control_p9, control_p10, control_p11, control_p12, control_p13;
uniform int instanceNumber;
uniform int pointsNumber;


layout(location = 0 )out vec2 texture_coord;



vec3 cubicBezier(float t, vec3 cp1, vec3 cp2, vec3 cp3, vec3 cp4) {
    return cp1 * pow((1 - t), 3) + cp2 * 3 * t * pow((1 - t), 2) + cp3 * 3 * pow(t, 2) * (1 - t) + cp4 * pow(t, 3);
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
        return vec3(intersectX, 0, intersectY);
    }

    return vec3(0, 0, 0);
}

vec3 get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y)
{
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return vec3(0,0,0); // Collinear
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return  vec3(0,0,0); // No collision

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return vec3(0,0,0); // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return vec3(0,0,0); // No collision
    // Collision detected
    t = t_numer / denom;
    
        float intX = p0_x + (t * s10_x);
 
       float intZ = p0_y + (t * s10_y);

    return vec3(intX, 0, intZ);
}


void createInBetweenMountain(vec3 intersectPoint, vec3 leftLowerPoint,vec3 leftLowerPointOffset, vec3 leftUpperPoint,vec3 leftUpperPointOffset, vec3 rightLowerPoint,vec3 rightLowerPointOffset, vec3 rightUpperPoint, vec3 rightUpperPointOffset) {


gl_Position = Projection * View * vec4(leftLowerPoint + leftLowerPointOffset, 1);
texture_coord = vec2(0, 0);
EmitVertex();



gl_Position = Projection * View * vec4(rightLowerPoint+rightLowerPointOffset, 1);
texture_coord = vec2(1, 0);
EmitVertex();

gl_Position = Projection * View * vec4(leftUpperPoint + leftUpperPointOffset, 1);
texture_coord = vec2(0, 1);
EmitVertex();


//rect to end of intersection


gl_Position = Projection * View * vec4(rightUpperPoint+rightUpperPointOffset, 1);
texture_coord =vec2(1,1);
EmitVertex();

EndPrimitive();

}


void main() {


vec3 p4,p5,p6,p7,p8,p9,p10,p11,p12,p13;
for (float t = 0.0; t < 1; t += 1.0 / pointsNumber) {

//middle branch
p4 = cubicBezier(t, control_p4, control_p5, control_p6, control_p7);
p5 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p5, control_p6, control_p7);
p6 = cubicBezier(t, control_p4, control_p5, control_p6, control_p7);
p7 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p5, control_p6, control_p7);

//right branch
p4 = cubicBezier(t, control_p4, control_p8, control_p9, control_p10);
p8 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p8, control_p9, control_p10);
p9 = cubicBezier(t, control_p4, control_p8, control_p9, control_p10);
p10 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p8, control_p9, control_p10);

//left branch
p4 = cubicBezier(t, control_p4, control_p11, control_p12, control_p13);
p11 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p11, control_p12, control_p13);
p12 = cubicBezier(t, control_p4, control_p11, control_p12, control_p13);
p13 = cubicBezier(t + 1.0 / pointsNumber, control_p4, control_p11, control_p12, control_p13);

vec3 lastPOffset;
if (t==0) {
lastPOffset = vec3(0,-0.9,0);
}else {

lastPOffset = vec3(0,0,0);
}

createInBetweenMountain(p4+vec3(0,-1,-1),p11,vec3(3,0,0),p12,vec3(3,0,0),p5,vec3(0,0,0),p6,lastPOffset);

createInBetweenMountain(p4+vec3(0,-1,-1),p5,vec3(3,0,0),p6,vec3(3,0,0),p8,vec3(0,0,0),p9,lastPOffset);
}

}