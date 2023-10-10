
#version 430 core

// light data
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 inF_Normal;
in vec3 inF_LightDir;
in vec3 inF_HalfwayVector;

in float inF_YPosition;
in vec3 inF_FragPos;

uniform vec3 u_CameraWorldPos;

uniform Light u_LightData;
uniform float u_AmbientStrength;
uniform vec3 u_OceanColor;

out vec4 out_FragColor;

void main()
{

    vec3 unitNormal = normalize(inF_Normal);
    vec3 unitLightDir = normalize(inF_LightDir);
    vec3 unitHalfwayVector = normalize(inF_HalfwayVector);
    
    vec4 emissiveColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 ambientColor = vec4(0.0f, 0.6f, 0.7f, 1.0f);
    vec4 diffuseColor = vec4(0.5f, 0.6f, 0.7f, 1.0f);
    vec4 specularColor = vec4(1.0f, 0.2f, 0.0f,  1.0f);

    float emissiveStrength = 0.1f;
    float ambientStrength = 0.3f;
    float diffuseStrength = 0.3f;
    float specularStrength = 1.8f;

    float dotProd = dot(unitNormal, unitLightDir);
    bool isFacing = dotProd > 0.0f;
    
    const vec3 color = vec3(0.2, 0.4, 0.65);
    out_FragColor = vec4(color, 1.0f);

    /*
    out_FragColor = 
        emissiveColor * emissiveStrength +
        ambientColor * ambientStrength +
        diffuseColor * diffuseStrength * max(dotProd, 0.0f) +
        (isFacing ? 
            specularColor * specularStrength * max(pow(dot(unitNormal, unitHalfwayVector), 120.0f), 0.0f) : 
            vec4(0.0f, 0.0f, 0.0f, 0.0f)
        );
    */



}


