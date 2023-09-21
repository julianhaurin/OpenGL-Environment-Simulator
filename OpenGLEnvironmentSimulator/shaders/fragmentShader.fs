
#version 430 core

in vec3 inF_Norm;
in vec3 inF_FragPos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

out vec4 out_FragColor;

void main()
{

    const vec3 normal = normalize(inF_Norm);
    const vec3 lightDirection = normalize(u_LightPosition - inF_FragPos);
    const vec3 viewDirection = normalize(u_ViewPosition - inF_FragPos);
    const vec3 reflectDirection = reflect(-lightDirection, normal);

    // ambient lighting
    const float ambientStrength = 0.1f;
    const vec3 ambientLighting = ambientStrength * u_LightColor;

    // diffuse lighting
    const float diffuse = max(dot(normal, lightDirection), 0.0f);
    const vec3 diffuseLighting = diffuse * u_LightColor;

    // specular lighting
    const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256); // 256 = shininess value
    const float specularStrength = 0.5f;
    const vec3 specularLighting = specularStrength * spec * u_LightColor;

    // result
    const vec3 result = (ambientLighting + diffuseLighting + specularLighting) * u_ObjectColor;
    out_FragColor = vec4(result, 0.1f);
}
