
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
out vec3 inF_LightDir;
out vec3 inF_HalfwayVector;

out float inF_YPosition;
out vec3 inF_FragPos;

void main() 
{
	// Calculate position
	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f);

	// Calculate Out Values //
	vec3 unitNorm = normalize(inV_Norm);
	inF_Normal = unitNorm; // (inverse(transpose(u_View * u_Model)) * vec4(unitNorm, 0.0)).xyz;
	
	vec4 viewHelper = u_View * u_Model * vec4(inV_Pos, 1.0f);
	inF_LightDir = normalize((u_View * vec4(u_LightData.position, 1.0f)).xyz - viewHelper.xyz);

	inF_HalfwayVector = inF_LightDir + normalize(-viewHelper.xyz);

	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f));
	inF_YPosition = inV_Pos.y;

}