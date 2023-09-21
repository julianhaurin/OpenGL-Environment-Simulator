
#version 430 core

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 inF_Norm;
out vec3 inF_FragPos;

void main() 
{

	inF_Norm = mat3(transpose(inverse(u_Model))) * inV_Norm; // normal matrix, learnopengl.com/Lighting/Basic-Lighting
	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f)); // world position of frag
	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

}