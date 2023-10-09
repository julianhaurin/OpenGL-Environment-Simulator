
#version 430 core

layout (location = 0) in vec3 inV_Pos;
layout (location = 1) in vec3 inV_Norm;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_LightPos;

out vec3 inF_Normal;
out vec3 inF_LightDir;
out vec3 inF_HalfwayVector; // idk

out float inF_YPosition;
out vec3 inF_FragPos;

void main() 
{
	// Calculate position
	gl_Position = u_Projection * u_View * u_Model * vec4(inV_Pos, 1.0f); // * inV_Normal;

	// Calculate Out Values //
	vec3 unitNorm = normalize(inV_Norm);
	inF_Normal = unitNorm; // (inverse(transpose(u_View * u_Model)) * vec4(unitNorm, 0.0)).xyz;
	
	vec4 viewHelper = u_View * u_Model * vec4(inV_Pos, 1.0);
	inF_LightDir = normalize((u_View * vec4(u_LightPos, 1.0)).xyz - viewHelper.xyz);

	inF_HalfwayVector = inF_LightDir + normalize(-viewHelper.xyz);

	inF_FragPos = vec3(u_Model * vec4(inV_Pos, 1.0f));
	inF_YPosition = inV_Pos.y;

}