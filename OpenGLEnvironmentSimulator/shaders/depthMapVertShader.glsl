
// Vertex shader used for shadow mapping

#version 430 core

layout (location = 0) in vec3 inV_Pos;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_LightSpaceMatrix * u_Model * vec4(inV_Pos, 1.0f);
}  