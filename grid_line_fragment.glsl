#version 330 core

out vec4 fColor;

void main()
{
    vec3 color = vec3(0.0f, 1.0f, 0.0f);

    fColor = vec4(color, 1.0f);
}