#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 cubePos;
uniform vec3 cameraPos;

in vec3 FragPos;
in vec2 TexCoord;
uniform sampler2D grass;

in vec3 Normal;
uniform Material material;
uniform Light light;
void main() {

        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

        // diffuse
        vec3 norm = normalize(Normal);
        // vec3 lightDir = normalize(light.position - FragPos);
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

        // specular
        vec3 viewDir = normalize(cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

        vec3 result = ambient + diffuse + specular;

    float dist = length((cameraPos - cubePos).xyz);
    float maxDist = 50;
    float minDist = 20;
    vec3 fogcolor = vec3(0.78,0.78,0.82);
    float fogFactor = (maxDist - dist) / (maxDist - minDist);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
   // vec3 texcolor = texture(grass, TexCoord).rgb;
    //vec3 color = mix(fogcolor, texcolor, fogFactor);

    vec3 color = mix(fogcolor, result, fogFactor);
    FragColor = vec4(color, 1.0);
}
