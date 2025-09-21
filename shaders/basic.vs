#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec4 out_color;

uniform mat4 u_in_model;
uniform mat4 u_in_model_view_proj;

void main() {
    gl_Position = u_in_model_view_proj * vec4(in_position, 1.0f);
    out_color = vec4(in_normal, 1.0f);
}