#version 330 core

layout(location = 0) out vec4 main_color;

in vec4 out_color;

void main() {
    main_color = out_color;
}