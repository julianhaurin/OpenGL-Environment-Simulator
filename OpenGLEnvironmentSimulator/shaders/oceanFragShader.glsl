
#version 430 core

// light data
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 inF_Normal;
in vec3 inF_FragPos;
in float inF_FragHeight;

uniform vec3 u_CameraWorldPos;

uniform Light u_LightData;
uniform float u_AmbientStrength;
uniform vec3 u_OceanColor;

out vec4 out_FragColor;

void main()
{

    const vec3 unitNormal = normalize(inF_Normal);
    const vec3 unitLightDir = normalize(u_LightData.position);
    const vec3 cameraDirection = normalize(u_CameraWorldPos - inF_FragPos);
    const vec3 reflectDirection = reflect(-unitLightDir, -unitNormal);

    float ambient = 0.3f;
    const vec3 ambientLighting = ambient * u_LightData.ambient;

    float diffuse = max(dot(unitNormal, unitLightDir), 0.0f);
    const vec3 diffuseLighting = diffuse * u_LightData.diffuse;

    float specular = pow(max(dot(cameraDirection, reflectDirection), 0.0), 256);
    const vec3 specularLighting = specular * u_LightData.specular;

    float dotProd = dot(unitNormal, unitLightDir);
    bool isFacing = dotProd > 0.0f;
    
    // const vec3 color = vec3(0.2, 0.4, 0.65);
    // const vec3 result = (ambientLighting + diffuseLighting + specularLighting);
    // out_FragColor = vec4(result * color, 1.0f);

    // inF_FragHeight
    const float heightMod = max(0.0f, (inF_FragHeight + 5) / 8);
    const vec3 oceanColor = vec3(0.0f, 0.5f + (heightMod / 4), 0.8f - (heightMod / 9)) * heightMod * 0.8f;

    const vec3 result = oceanColor; // * (specularLighting + vec3(0.5, 0.5, 0.5));
    out_FragColor = vec4(result, 1.0f);

}


