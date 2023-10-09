
#version 430 core

in vec3 inF_Normal;
in vec3 inF_LightDir;
in vec3 inF_HalfwayVector;

in vec3 inF_FragPos;
in float inF_YPosition;

uniform vec3 u_CameraWorldPos;

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
    bool isFacing = dotProd > 0.0f;

    
    out_FragColor = 
        emissiveColor * emissiveStrength +
        ambientColor * ambientStrength +
        diffuseColor * diffuseStrength * max(dotProd, 0.0f) +
        (isFacing ? 
            specularColor * specularStrength * max(pow(dot(unitNormal, unitHalfwayVector), 120.0f), 0.0f) : 
            vec4(0.0f, 0.0f, 0.0f, 0.0f));
    

    vec3 viewDir = normalize(u_CameraWorldPos - inF_FragPos);
    float fresnelEffect = 0.2f + 0.91f * pow(1.0f - dot(unitNormal, viewDir), 5.0f);

    vec3 skyColor = vec3(3.2f, 9.6f, 12.8f);
    vec3 oceanColor = vec3(0.004f, 0.016f, 0.047f);

    vec3 sky = fresnelEffect * skyColor;
    float diffuse = clamp(dot(unitNormal, normalize(-unitLightDir)), 0.0f, 1.0f);
    vec3 water = (1.0f - fresnelEffect) * oceanColor * skyColor * diffuse;
    
    vec3 color = sky + water;

    // out_FragColor = vec4(color, 1.0f);

    /*
    if (inF_YPosition <= 0) {
        color.z += color.z * abs(inF_YPosition) * 100.0f;
    }
    */

    /*
    float colorModifer = 10 * inF_YPosition + 75.0f;
    out_FragColor = vec4(0.1f, colorModifer / 100.0f, 0.9f - colorModifer / 900.0f, 1.0f);
    out_FragColor *= vec4(1.0f, 0.8f, 1.1f, 1.0f);
    */

    //sky *= out_FragColor.z;
    //out_FragColor *= vec4(sky, 1.0f);


}


