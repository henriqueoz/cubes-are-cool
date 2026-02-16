#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    FragColor = vec4(result, 1.0);
}