#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPos;

void main() {
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);

    fragPos = vec3(model * vec4(vertexPosition, 1.0));
    fragNormal = vertexNormal;
}