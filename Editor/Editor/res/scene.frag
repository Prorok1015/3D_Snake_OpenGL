#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
    if (gl_FrontFacing) {
        FragColor.r = 1.f;
    } else {
        FragColor.b = 1.f;
    }
}