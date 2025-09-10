#version 330 core

layout(location = 0) out vec4 main_color;

in vec4 out_color;//für die vertex farben
in vec2 out_textCoord;//für die texturkoordinaten

uniform sampler2D u_in_texture;//textur input

uniform vec4 u_in_color;//color input

void main() {
    vec4 texColor = texture(u_in_texture, out_textCoord);//texturfarbe
    main_color = texColor;
    //main_color = u_in_color;
}