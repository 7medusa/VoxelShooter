#version 330 core

layout(location = 0) out vec4 main_color;

in vec3 out_normal;
in vec3 out_position;

void main() {
    vec3 color = vec3(1.0, 1.0, 1.0);
    vec3 view = normalize(-out_position);
    vec3 light = normalize(vec3(-100.0, 100.0, 100.0));//lichtrichtung
    vec3 normal = normalize(out_normal);
    vec3 reflection = reflect(-light, normal);

    vec3 ambient = color * 0.2;//ambient licht
    vec3 diffuse = max(dot(normal, light), 0.0) * color;//diffuse licht
    vec3 specular = pow(max(dot(reflection, view), 0.0), 4.0) * color;
    main_color = vec4(ambient + diffuse + specular, 1.0);
}