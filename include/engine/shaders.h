#pragma once

inline const char* vertexShader2D = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;
out vec2 UV;
uniform vec2 pos; uniform vec2 size; uniform vec2 screen; uniform float angle;
void main(){
    float s=sin(angle); float c=cos(angle);
    vec2 p=vec2(aPos.x*c-aPos.y*s,aPos.x*s+aPos.y*c);
    p=p*size+pos;
    vec2 ndc=(p/screen)*2.0-1.0;
    ndc.y=-ndc.y;
    gl_Position=vec4(ndc,0,1);
    UV=aUV;
}
)glsl";

inline const char* vertexShader3D = R"glsl(
#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;

out vec2 UV;

uniform float fov;
uniform float aspect;

void main()
{
    float near = 0.1;
    float far = 100.0;

    float f = 1.0 / tan(radians(fov) * 0.5);

    mat4 proj = mat4(
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far+near)/(near-far), -1,
        0, 0, (2*far*near)/(near-far), 0
    );

    gl_Position = proj * vec4(aPos, 1.0);

    UV = aUV;
}
)glsl";

inline const char* fragmentShader = R"glsl(
#version 330 core
in vec2 UV;
out vec4 FragColor;
uniform sampler2D tex;
uniform float alpha;
void main(){
    vec4 c = texture(tex, UV);
    FragColor = vec4(c.rgb, c.a * alpha);
}
)glsl";