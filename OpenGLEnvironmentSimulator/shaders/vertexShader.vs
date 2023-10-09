
// Default vertex shader used in Model.h

#version 430 core

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;
layout (location = 2) in vec2 inV_TexCoords;

// render matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// fragment shader input values
out vec3 inF_Norm; // fragment normals
out vec3 inF_FragPos;
out vec2 inF_TexCoords;

// learnopengl.com/Lighting/Basic-Lighting
void main() 
{

	inF_Norm = mat3(transpose(inverse(u_Model))) * inV_Norm; // normal vector
	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f)); // world position of frag
	inF_TexCoords = inV_TexCoords; // texture coordinates

	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

}