#version 330 core

layout(location = 0) out vec4 main_color;

in vec4 out_color;//für die vertex farben

uniform vec4 u_color;//color input

void main() {
    main_color = u_color;
    //main_color = out_color;
}