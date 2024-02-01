#version 330 core

in vec3 fcolor;

out vec4 FragColor;

in vec3 fcfcotxe;

void main() {

	FragColor = vec4(fcolor*fcfcotxe, 1);
}

