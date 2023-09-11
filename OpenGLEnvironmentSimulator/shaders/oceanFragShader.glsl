
#version 430 core

in vec3 inF_Normal;
in vec3 inF_LightDir;
in vec3 inF_HalfwayVector;

in vec3 inF_FragPos;

uniform float u_AmbientStrength;
uniform vec3 u_OceanColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

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
    bool isFacing = dotProd > 0.0;

    out_FragColor = 
        emissiveColor * emissiveStrength +
        ambientColor * ambientStrength +
        diffuseColor * diffuseStrength * max(dotProd, 0.0f) +
        (isFacing ? 
            specularColor * specularStrength * max(pow(dot(unitNormal, unitHalfwayVector), 120.0f), 0.0f) : 
            vec4(0.0f, 0.0f, 0.0f, 0.0f));

    out_FragColor.a = 1.0f;

    out_FragColor = vec4(u_OceanColor, 1.0f);

}

// ***************************************************************
// OpenGL Lighting Tutorial - Phong

// ambient lighting
// vec3 ambientLighting = u_LightColor * u_AmbientStrength; 

// normlize normal vector and calculate light direction
// vec3 unitNorm = normalize(inF_Norm);
// vec3 unitLightDir = normalize((View * vec4(light_position, 1.0)).xyz - v.xyz); // normalize(u_LightPos - inF_FragPos);

// diffuse lighting
// vec3 diffuseLighting = u_LightColor * max(dot(unitNorm, unitLightDir), 0.0f);

// vec3 result = (ambientLighting + diffuseLighting) * u_OceanColor; // vec3(1.0f, 1.0f, 1.0f) * diffuse;
// ***************************************************************

