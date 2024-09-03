#version 420 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}