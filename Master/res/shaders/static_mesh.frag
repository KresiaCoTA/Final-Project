#version 330 core

in vec2 v_tex_coord;

uniform sampler2D u_texture;
uniform float u_tex_factor;
uniform vec4 u_color;

out vec4 f_color;

void main() {
    vec4 textColor = mix(u_color, texture(u_texture, v_tex_coord), u_tex_factor);
    if(textColor.a < 0.1)
        discard;
    f_color = textColor;
}
