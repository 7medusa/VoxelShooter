#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textCoord;
layout(location = 2) in vec4 in_color;

out vec4 out_color;
out vec2 out_textCoord;

uniform mat4 in_model;

void main() {
    gl_Position = in_model * vec4(in_position, 1.0f);
    out_color = in_color;
    out_textCoord = in_textCoord;
}