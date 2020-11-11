#version 330 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in float vColor;

out float Color;

void main()
{
    gl_Position = vec4(vPosition, 1.0f);
    Color = vColor;
}