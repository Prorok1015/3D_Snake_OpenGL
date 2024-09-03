#version 420 core
out vec4 FragColor;

layout (std140, binding = 1) uniform SunLight 
{
    // vec3 have size 16 as vec4. you should remember it.
    vec3 lightColor; // 16
    vec3 lightPos;   // 16 + 16
};

varying struct PiplineStruct
{
    vec2 UV;
    vec3 Normal;
    vec3 FragPos;
} out_ps;

uniform sampler2D texture_diffuse1;

void main()
{    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.xyz;
  	
    // diffuse
    vec3 norm = normalize(out_ps.Normal);
    vec3 lightDir = normalize(lightPos - out_ps.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.xyz;
            
    vec4 result = vec4(ambient + diffuse, 1.0) * texture(texture_diffuse1, out_ps.UV);
    FragColor = result;
}