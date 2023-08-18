
#version 330 core

out vec4 out_FragColor;

uniform float u_Color;

void main()
{
    out_FragColor = vec4(0.8f, 1.0f, u_Color, 0.1f);
}
