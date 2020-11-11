#version 330 core

flat in int color;

out vec4 fColor;

void main()
{
    fColor = vec4(vec3(float(color)), 1.0f);
}