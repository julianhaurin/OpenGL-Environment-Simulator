
#version 430 core

layout (location = 0) in vec3 inV_Pos;

out vec3 inF_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    inF_TexCoords = inV_Pos;
    vec4 pos = u_Projection * u_View * vec4(inV_Pos, 1.0f);
    gl_Position = pos.xyww;
}  