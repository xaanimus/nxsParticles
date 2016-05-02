varying highp vec2 v_uv;
varying highp vec3 clr;

uniform sampler2D particle_tex;

void main() {
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    vec4 c = texture2D(particle_tex, v_uv);

    c.x *= clr.x;
    c.y *= clr.y;
    c.z *= clr.z;

    float alpha = (c.x + c.y + c.z) / 3.0;
    gl_FragColor = vec4(c.xyz, alpha);
}
