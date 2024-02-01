#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 cfcotxe;

out vec3 fcolor;
out vec3 fcfcotxe;

void main() {
    fcolor = color;
    fcfcotxe = cfcotxe;
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
