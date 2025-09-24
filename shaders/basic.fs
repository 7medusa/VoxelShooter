#version 330 core

layout(location = 0) out vec4 main_color;

in vec3 out_normal;
in vec3 out_position;

uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_emissive;
uniform float u_shininess;

void main() {
    vec3 view = normalize(-out_position);
    vec3 light = normalize(vec3(100.0, 100.0, 100.0));//lichtrichtung
    //vec3 light = normalize(vec3(1, 1, 1));//lichtrichtung
    vec3 normal = normalize(out_normal);
    vec3 reflection = reflect(-light, normal);

    vec3 ambient = u_diffuse * 0.2;//ambient licht
    vec3 diffuse = max(dot(normal, light), 0.01) * u_diffuse;//diffuse licht
    vec3 specular = pow(max(dot(reflection, view), 0.01), u_shininess) * u_specular;
    main_color = vec4(ambient + diffuse + specular + u_emissive, 1.0);
}