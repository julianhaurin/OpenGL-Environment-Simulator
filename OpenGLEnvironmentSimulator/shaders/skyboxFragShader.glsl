
#version 430 core

out vec4 out_FragColor;

in vec3 inF_TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    out_FragColor = texture(u_Skybox, inF_TexCoords);
}