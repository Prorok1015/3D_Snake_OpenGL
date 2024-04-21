#version 420 core
out vec4 FragColor;

varying vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    if (gl_FrontFacing) {
        //FragColor.r = 1.f;
    } else {
        //FragColor.b = 1.f;
    }
    FragColor = texture(texture_diffuse1, vec2(TexCoords.x/16, -TexCoords.y/16));

}