#version 430
layout(location = 0)
in vec2
texture_coord;

uniform sampler2D
colorTexture;
uniform sampler2D
depthTexture;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
out vec4 out_color;
float blurFactor;
float zNear = 0.08;
float zFar = 10.0;
float nearPlane = 2;
float farPlane = 100.0;
layout(location = 0)


float linearDepth(float depthSample) {
    float depthNDCCoordonates = depthSample * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - depthNDCCoordonates * (zFar - zNear));
}

float depth() {
    float depthTextureOnX = texture(depthTexture, texture_coord).x;
    float linearizedDepth=linearDepth(depthTextureOnX);
    return linearizedDepth;
}


vec3 color() {
    return texture(colorTexture, texture_coord).xyz;
}


vec3 quickGaussianBlur(float blurFactor) {
    vec2 tex_offset = 1.0 / textureSize(colorTexture, 0) * blurFactor;
    vec3 result = texture(colorTexture, texture_coord).rgb * weight[0];

    for (int i = 1; i < 5; ++i) {
        result += texture(colorTexture, texture_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(colorTexture, texture_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
    return result;
}

void main() {

    blurFactor = depth()/8;

    vec3 color = color();

    out_color = vec4(quickGaussianBlur(blurFactor), 1);
}
