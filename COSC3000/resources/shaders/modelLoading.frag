
#version 330 core

in vec2 TexCoords;

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, TexCoords));
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, TexCoords));
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    vec3 specular = light.specular * spec * vec3(texture( texture_specular, TexCoords));
    
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    color = vec4(ambient + diffuse + specular, 1.0f);
}


//    color = vec4( texture( texture_diffuse, TexCoords ));
