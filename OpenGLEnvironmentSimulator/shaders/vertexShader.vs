
#version 430 core

layout (location = 0) in vec3 pos;
//layout (location = 1) in vec3 norm;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() 
{
	gl_Position = u_Projection * u_View * u_Model * vec4(pos, 1.0f);

}