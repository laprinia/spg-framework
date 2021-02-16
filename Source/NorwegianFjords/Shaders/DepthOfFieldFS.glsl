#version 430
layout(location = 0) in vec2 texture_coord;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform vec2 screenSize;
out vec4 out_color;

float blurFactor;
float zNear = 0.08;
float zFar = 10.0;
float nearPlane = 2;
float farPlane = 100.0;

int kernelNumber = 12;
vec2 twelveKernelBase[] =
        {
                {1.0,  0.0},
                {0.5,  0.866},
                {-0.5, 0.866},
                {-1.0, 0.0},
                {-0.5, -0.866},
                {0.5,  -0.866},
                {1.5,  0.866},
                {0,    0.1216216},
                {-1.5, 0.866},
                {-1.5, 0.866},
                {0,    -0.1216216},
                {1.5,  -0.866},
        };

float linearDepth(float depthSample) {
    float depthNDCCoordonates = depthSample * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - depthNDCCoordonates * (zFar - zNear));
}

float depth() {
    float depthTextureOnX = texture(depthTexture, texture_coord).x;
    float linearizedDepth = linearDepth(depthTextureOnX);
    return linearizedDepth;
}

float getBlurFactor() {
    float blur = 0;
    if (depth() <= nearPlane && depth() >= farPlane) {
        blur = 0;
    } else if (depth() > nearPlane) {
        blur = clamp(depth(), nearPlane, nearPlane + zNear);
        blur = (blur - nearPlane) / zNear;
    } else if (depth() < farPlane) {
        blur = clamp(depth(), farPlane - zFar, farPlane);
        blur = (farPlane - blur) / depth();
    }
    return blur;
}

vec3 color() {
    return texture(colorTexture, texture_coord).xyz;
}

void main() {
    float intensityModifier=2;
    vec3 originalColor = color();
    vec3 blurredOutput = vec3(0, 0, 0);
    for (int i = 0; i < kernelNumber; i++) {
        vec2 textureOffset = vec2(twelveKernelBase[i].x / screenSize.x * intensityModifier,
                             twelveKernelBase[i].y / screenSize.y * intensityModifier);
        vec4 currentTexture = texture2D(colorTexture, texture_coord + textureOffset);
        blurredOutput += mix(originalColor.rgb, currentTexture.rgb, getBlurFactor());
    }

    out_color = vec4(blurredOutput / kernelNumber, 1);
}
