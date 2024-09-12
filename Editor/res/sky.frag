#version 420 core
in vec3 TexCoords;

struct Material {
    samplerCube diffuse;
    samplerCube specular;
    samplerCube ambient;
    float shininess;
}; 
  
uniform Material material;

void main()
{    
    gl_FragColor = texture(material.diffuse, TexCoords);
}