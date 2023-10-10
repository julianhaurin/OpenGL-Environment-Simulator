
// Default fragment shader used in Model.h

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

// fragment shader input values
in vec3 inF_Norm;
in vec3 inF_FragPos;
in vec2 inF_TexCoords;

uniform vec3 u_ViewPosition;

// lighting and material values
uniform Light u_Light;
uniform Material u_Material;

// texture data
uniform sampler2D u_TextureData;

// output frag color
out vec4 out_FragColor;

void main()
{

    const vec3 normal = normalize(inF_Norm);
    const vec3 lightDirection = normalize(u_Light.position - inF_FragPos);
    const vec3 viewDirection = normalize(u_ViewPosition - inF_FragPos);
    const vec3 reflectDirection = reflect(-lightDirection, normal);

    // ambient lighting //
    const float ambient = 1.0f;
    const vec3 ambientLighting = ambient * u_Light.ambient;

    // diffuse lighting //
    const float diffuse = max(dot(normal, lightDirection), 0.0f);
    const vec3 diffuseLighting = diffuse * u_Light.diffuse;

    // specular lighting //
    const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256); // 256 = shininess value
    const vec3 specularLighting = spec * u_Light.specular;

    // result //
    const vec3 result = (ambientLighting + diffuseLighting + specularLighting);
    out_FragColor = texture(u_TextureData, inF_TexCoords) * vec4(result, 0.1f);

}
