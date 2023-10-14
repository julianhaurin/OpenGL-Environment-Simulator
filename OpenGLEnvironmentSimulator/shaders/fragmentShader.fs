
// Default fragment shader used in Model.h

#version 430 core

// number of individual light sources in scene
#define LIGHTS_COUNT 2

// light data
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
	float linear;
	float quadratic;

};

// material data
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// fragment shader input values
in vec3 inF_Norm;
in vec3 inF_FragPos;
in vec2 inF_TexCoords;

uniform vec3 u_ViewPosition;

// lighting and material values
uniform Light u_LightSources[LIGHTS_COUNT];
uniform Material u_Material;

// texture data
uniform sampler2D u_TextureData;

// output frag color
out vec4 out_FragColor;

// calculates all point lights in scene
vec3 calculatePointLight(Light in_lightData, vec3 in_normal, vec3 in_fragPos, vec3 in_camDir) {

    const vec3 lightDir = normalize(in_lightData.position - in_fragPos);
    const vec3 reflectDir = reflect(-lightDir, in_normal);

    // ambient
    vec3 ambientLighting = in_lightData.ambient * u_Material.ambient;

    // diffuse
    const float diffuse = max(dot(in_normal, lightDir), 0.0f);
    vec3 diffuseLighting = diffuse * in_lightData.diffuse * u_Material.diffuse;

    // specular
    const float specular = pow(max(dot(in_camDir, reflectDir), 0.0f), 256);
    vec3 specularLighting = specular * in_lightData.specular * u_Material.specular;

    // attenuation
    const float distanceToLight = length(in_fragPos - in_lightData.position);
    const float attenuation = 1.0f / 
        (in_lightData.constant + 
        in_lightData.linear * distanceToLight + 
        in_lightData.quadratic * (distanceToLight * distanceToLight));

    ambientLighting *= attenuation;
    diffuseLighting *= attenuation;
    specularLighting *= attenuation;

    return (ambientLighting + diffuseLighting + specularLighting);


}

void main()
{
    
    vec3 normal = normalize(inF_Norm);
    vec3 camDir = normalize(u_ViewPosition - inF_FragPos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < LIGHTS_COUNT; i++) {
        vec3 lightResult = calculatePointLight(u_LightSources[i], normal, inF_FragPos, camDir);
        result += lightResult;
    }
    

    // result //
    out_FragColor = texture(u_TextureData, inF_TexCoords) * vec4(result, 0.1f);

}
