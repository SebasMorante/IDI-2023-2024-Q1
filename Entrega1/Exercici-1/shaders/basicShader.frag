#version 330 core

out vec4 FragColor;
//////
in vec3 fcolor;
in vec3 fragcaspa;

//uniform vec3 cfaspa

void main() {
    vec3 colorfin = fcolor*fragcaspa;
    FragColor = vec4(colorfin, 1);
}
