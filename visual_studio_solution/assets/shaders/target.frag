//  my four raytracer by SONENEIS   | https://www.shadertoy.com/view/cdjSDd
#version 330 core

out vec4 FragColor;
in vec2 vTex;

uniform float uTime;

// --- minimal crosshair shader (project format) ---

mat2 rot(float a){
    float s = sin(a), c = cos(a);
    return mat2(c, -s, s, c);
}

void main()
{
    // vTex is expected in 0..1
    // build a centered uv like Shadertoy's: uv = (2*fragCoord - iResolution.xy) / iResolution.y
    // without resolution we approximate with a square-normalized centered space:
    vec2 uv = (vTex - 0.5) * 2.0;   // -1..1 (no aspect correction available)

    vec3 col = vec3(0.0);

    // --- crosshair (ported from your Shadertoy version) ---
    vec2 p = uv * 0.5;

    float b  = sin(uTime * 1.5) * 1.0075;
    float hb = sign(b) * pow(abs(b), 64.0);

    vec2 nuv = p * rot(hb);
    vec2 q   = (abs(nuv.x) < abs(nuv.y)) ? nuv.xy : nuv.yx;

    // vertical + horizontal bar
    float dBar = max(
        -(max(abs(q.x), abs(q.y)) - 0.005),
         max(abs(q.x) - 0.002, abs(q.y) - 0.01)
    );

    if (dBar < 0.0)
        col = vec3(1.0, 0.0, 0.0);

    // outer ring
    float dRing = max(
        -max(
            -(abs(p.y) - 0.0075 - abs(b * 0.01) + 0.005),
            -(abs(p.x) - 0.0075 - abs(b * 0.01) + 0.005)
        ),
        abs(length(p) - 0.02 - (abs(b*b*b*b*b) * 0.0025)) - 0.001
    );

    if (dRing < 0.0)
        col = vec3(1.0, 0.0, 0.0);

    float alpha = step(0.0001, length(col));
    FragColor = vec4(col, alpha);
}