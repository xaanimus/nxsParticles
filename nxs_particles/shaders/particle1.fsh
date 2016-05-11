varying highp vec2 v_uv;
varying highp vec3 clr;

uniform sampler2D particle_tex;

void main() {
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    vec4 c = texture2D(particle_tex, v_uv);
    c.r = clr.x * c.r;
    c.g = clr.y * c.g;
    c.b = clr.z * c.b;
    float alpha = (c.x + c.y + c.z) / 3.0;
    gl_FragColor = vec4(c.xyz, alpha);
}
