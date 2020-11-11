#version 330 core

in float Color;

out vec4 fColor;


void main()
{
    fColor = vec4(vec3(Color), 1.0f);
}