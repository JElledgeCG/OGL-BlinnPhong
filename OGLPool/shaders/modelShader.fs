#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

void main()
{   
    // ambient lighting. Since global illumination is very expensive, ambient lighting in this way
    // is a cheap alternative, though not as pretty.
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // for science: what if these are not normalized?
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular lighting
    float specularStrength = 0.75;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //FragColor = vec4(Normal, 1.0);
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(ambient + diffuse + specular, 1.0);
}