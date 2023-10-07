
#version 430 core

// light data
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// material data
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 inF_Norm;
in vec3 inF_FragPos;
in vec2 inF_TexCoords;
in vec4 inF_FragPosLightSpace;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;

uniform Light u_Light;
uniform Material u_Material;

uniform sampler2D u_TextureData;
uniform sampler2D u_ShadowMap;

out vec4 out_FragColor;

float calculateShadow() {
    
    // perform perspective divide
    vec3 projCoords = inF_FragPosLightSpace.xyz / inF_FragPosLightSpace.w;

    projCoords = projCoords * 0.5f + 0.5f; // transforms to [0,1] range

    // retrieve closest depth value
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;

    return shadow;

}

void main()
{

    const vec3 normal = normalize(inF_Norm);
    const vec3 lightDirection = normalize(u_Light.position - inF_FragPos);
    const vec3 viewDirection = normalize(u_ViewPosition - inF_FragPos);
    const vec3 reflectDirection = reflect(-lightDirection, normal);

    // ambient lighting //
    // const float ambientStrength = 0.1f;
    // const vec3 ambientLighting = ambientStrength * u_LightColor;
    const vec3 ambientLighting = u_Light.ambient;

    // diffuse lighting //
    const float diffuse = max(dot(normal, lightDirection), 0.0f);
    // const vec3 diffuseLighting = diffuse * u_LightColor;
    const vec3 diffuseLighting = diffuse * u_Light.diffuse;

    // specular lighting //
    const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256); // 256 = shininess value
    // const float specularStrength = 0.5f;
    // const vec3 specularLighting = specularStrength * spec * u_LightColor;
    const vec3 specularLighting = spec * u_Light.specular;

    // shadow calculation //
    float shadow = 1.0f; // calculateShadow();    

    // result //
    const vec3 result = (ambientLighting + (1.0f - shadow) + diffuseLighting + specularLighting);// * u_ObjectColor;
    // out_FragColor = vec4(result, 0.1f);

    out_FragColor = texture(u_TextureData, inF_TexCoords) * vec4(result, 0.1f);
}
