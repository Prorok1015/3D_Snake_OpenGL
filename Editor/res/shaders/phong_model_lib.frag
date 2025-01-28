layout (std140, binding = 0) uniform Matrices 
{
    mat4 projection;
    mat4 view;
    float time;
    vec3 cameraPosition;
};

layout (std140, binding = 1) uniform Light 
{
    // vec3 have size 16 as vec4. you should remember it.
    vec3 position;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
} light;

layout(binding = 0) uniform sampler2D diffuseTxm;

#ifdef USE_SPECULAR_MAP
layout(binding = 1) uniform sampler2D specularTxm;
#endif

uniform float shininess;
uniform vec4 diffuseColor;

vec3 calculatePhongModel(vec3 norm, vec3 materialColor, vec3 materialSpecular, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(cameraPosition - fragPos);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambientColor  = light.ambient  * materialColor;
    vec3 diffuseColor  = light.diffuse  * diff * materialColor;  
    vec3 specularColor = light.specular * spec * materialSpecular;
    
    return ambientColor + diffuseColor + specularColor;
}