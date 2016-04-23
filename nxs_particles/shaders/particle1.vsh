attribute highp vec3 verts;
varying highp vec4 color;
uniform highp mat4 mvp;

void main() {
    color = vec4(1.0,1.0,1.0,1.0);
    gl_Position = mvp * vec4(verts, 1.0);
    gl_PointSize = 1.0;
}
