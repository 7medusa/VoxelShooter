#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec3 out_normal;
out vec3 out_position;

uniform mat4 u_in_model_view_proj;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;

void main() {
    gl_Position = u_in_model_view_proj * vec4(in_position, 1.0f);
    out_normal = mat3(u_invModelView) * in_normal;
    out_position = vec3(u_modelView * vec4(in_position, 1.0f));
}