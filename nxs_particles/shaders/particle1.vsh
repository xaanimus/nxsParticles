attribute highp vec3 billboard_vert;
attribute highp vec3 loc;
attribute highp vec2 uv;

varying highp vec2 v_uv;
varying highp vec3 clr;

uniform highp mat4 mvp;
uniform highp vec3 camera_up;
uniform highp vec3 camera_right;

void main() {

    vec3 vert_world_pos = loc
            + billboard_vert.x * camera_right
            + billboard_vert.y * camera_up;

    gl_Position = mvp * vec4(vert_world_pos, 1.0);

    clr = loc;

    v_uv = uv;
}
