#version 330 core

uniform mat4 u_model_view;
uniform mat4 u_projection;
uniform vec4 u_clip_plane;

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec2 a_tex_coord;

out vec2 v_tex_coord;
out float gl_ClipDistance[1];

void main() {
    v_tex_coord = a_tex_coord;
    vec4 view_pos = u_model_view * a_pos;
    gl_Position = u_projection * view_pos;
    gl_ClipDistance[0] = dot(u_clip_plane, view_pos);
}
