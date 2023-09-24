
#version 430 core

// light data
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_Light;

// material data
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material u_Material;

in vec3 inF_Norm;
in vec3 inF_FragPos;
in vec2 inF_TexCoords;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
// uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

out vec4 out_FragColor;

void main()
{

    const vec3 normal = normalize(inF_Norm);
    const vec3 lightDirection = normalize(u_Light.position - inF_FragPos);
    const vec3 viewDirection = normalize(u_ViewPosition - inF_FragPos);
    const vec3 reflectDirection = reflect(-lightDirection, normal);

    // ambient lighting //
    // const float ambientStrength = 0.1f;
    // const vec3 ambientLighting = ambientStrength * u_LightColor;
    const vec3 ambientLighting = u_Material.ambient * u_Light.ambient;

    // diffuse lighting //
    const float diffuse = max(dot(normal, lightDirection), 0.0f);
    // const vec3 diffuseLighting = diffuse * u_LightColor;
    const vec3 diffuseLighting = u_Material.diffuse * diffuse * u_Light.diffuse;

    // specular lighting //
    const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess); // 256 = shininess value
    // const float specularStrength = 0.5f;
    // const vec3 specularLighting = specularStrength * spec * u_LightColor;
    const vec3 specularLighting = u_Material.specular * spec * u_Light.specular;

    // result //
    const vec3 result = (ambientLighting + diffuseLighting + specularLighting) * u_ObjectColor;
    out_FragColor = vec4(result, 0.1f);
}
