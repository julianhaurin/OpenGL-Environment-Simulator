
#version 430 core

// light data
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform Light u_LightData;

out vec3 inF_Normal;
out vec3 inF_FragPos;
out float inF_FragHeight;

void main() 
{
	// Calculate fragment input values
	inF_Normal = (inverse(transpose(u_View * u_Model)) * vec4(normalize(inV_Norm), 0.0)).xyz;
	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f));
	inF_FragHeight = inV_Pos.y;

	// Calculate position
	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

}