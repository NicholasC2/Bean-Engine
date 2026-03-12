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
layout(location=0) in vec3 aPos; // Note: vec3 for 3D
layout(location=1) in vec2 aUV;
out vec2 UV;
uniform float fov;
uniform float aspect;
void main(){
    // Simple perspective math: x' = x/z, y' = y/z
    float f = 1.0 / tan(fov * 0.5 * 0.0174533);
    vec4 p = vec4(aPos.x * f, aPos.y * f * aspect, aPos.z, aPos.z);
    gl_Position = vec4(p.x, p.y, (p.z * 0.01), p.w); // Basic clip space
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