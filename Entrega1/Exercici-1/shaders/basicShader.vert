#version 330 core

in vec3 vertex;
in vec3 color;


uniform mat4 TG;

uniform vec3 cfaspa;

out vec3 fragcaspa;
out vec3 fcolor;

void main()  {
    fcolor = color;
    fragcaspa = cfaspa;
    gl_Position = TG * vec4 (vertex, 1.0);
}
