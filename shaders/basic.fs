#version 330 core

layout(location = 0) out vec4 main_color;

in vec4 out_color;//für die vertex farben

uniform sampler2D u_in_texture;//textur input

uniform vec4 u_in_color;//color input

void main() {
    //main_color = texColor;
    //main_color = u_in_color;
    main_color = out_color;
}