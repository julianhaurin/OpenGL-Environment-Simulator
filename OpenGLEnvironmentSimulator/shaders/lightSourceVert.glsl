
// Vertex shader for light source

#version 430

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;
layout (location = 2) in vec2 inV_TexCoords;

// render matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	
	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

}