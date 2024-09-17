#version 420 core
in vec3 TexCoords;
  
layout(binding = 0) uniform samplerCube diffuse;

void main()
{    
    gl_FragColor = texture(diffuse, TexCoords);
}