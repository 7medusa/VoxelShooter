#version 330 core

layout(location = 0) out vec4 main_color;

in vec3 out_normal;
in vec3 out_position;
in vec2 out_tex_coord;

uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_emissive;
uniform float u_shininess;
uniform sampler2D u_diffuse_map;

float lightStrength = 2.0;

void main() {
    vec4 diffuseColor = texture(u_diffuse_map, out_tex_coord);
    if(diffuseColor.w < 0.9) {
        discard;
    }

    vec3 view = normalize(-out_position);
    vec3 light = normalize(vec3(-10.0, -10.0, 10.0));//lichtrichtung
    //vec3 light = normalize(vec3(0.0, 0.0, 0.0));//licht aus
    vec3 normal = normalize(out_normal);
    vec3 reflection = reflect(-light, normal);

    vec3 ambient = u_diffuse * 0.2;//ambient licht
    vec3 diffuse = max(dot(normal, light*lightStrength), 0.01) * u_diffuse;//diffuse licht
    vec3 specular = pow(max(dot(reflection, view), 0.01), u_shininess) * u_specular;
    main_color = vec4((ambient + diffuse) * diffuseColor.xyz + specular + (u_emissive*5), 1.0);
}
