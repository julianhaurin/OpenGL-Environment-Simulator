
#version 430 core

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;
layout (location = 2) in vec2 inV_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix; // for shadow mapping

out vec3 inF_Norm;
out vec3 inF_FragPos;
out vec2 inF_TexCoords;
out vec4 inF_FragPosLightSpace; // frag position from perspective of light

void main() 
{

	inF_Norm = mat3(transpose(inverse(u_Model))) * inV_Norm; // normal matrix, learnopengl.com/Lighting/Basic-Lighting
	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f)); // world position of frag
	inF_TexCoords = inV_TexCoords;
	inF_FragPosLightSpace = u_LightSpaceMatrix * vec4(inF_FragPos, 1.0f);

	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

}