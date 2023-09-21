
#version 430 core

in vec3 inF_Norm;

out vec4 out_FragColor;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main()
{

    // ambient lighting
    const float ambientStrength = 0.1f;
    const vec3 ambientLighting = ambientStrength * u_LightColor;

    // diffuse lighting

    
    // specular lighting

    const vec3 result = ambientLighting * u_ObjectColor;
    out_FragColor = vec4(result, 0.1f);
}
