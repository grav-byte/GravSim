#version 330 core

in vec2 vTex;          // UVs (0..1)
out vec4 FragColor;

uniform vec2 uResolution;  // framebuffer size in pixels
uniform int uDirection;    // 0=up,1=right,2=down,3=left
uniform float uOffset;    // offset
uniform vec4 uColor;       // tint

float line(vec2 p, vec2 a, vec2 b, float w)
{
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return smoothstep(w, 0.0, length(pa - ba * h));
}

void main()
{
    // reconstruct fragment coordinates from UV
    vec2 fragCoord = vTex * uResolution;

    // normalize to [-aspect,aspect] x [-1,1]
    vec2 p = (fragCoord - 0.5 * uResolution) / uResolution.y;

    vec4 col = vec4(0.0);

    // basis vectors per direction
    vec2 dir;
    vec2 normal;

    if (uDirection == 0) {        // up
        dir = vec2(1, 0);
        normal = vec2(0, 1);
    }
    else if (uDirection == 1) {   // right
        dir = vec2(0, 1);
        normal = vec2(1, 0);
    }
    else if (uDirection == 2) {   // down
        dir = vec2(1, 0);
        normal = vec2(0, -1);
    }
    else {                        // left
        dir = vec2(0, 1);
        normal = vec2(-1, 0);
    }

    // main constraint line
    float mainThickness = 0.002;
    col += line(p, -dir, dir, mainThickness);

    // hatch pattern
    float spacing = 0.03;
    float thickness = 0.001;
    float len = .05;

    vec2 hatchDir = normalize(dir + normal);

    float res = 0.0f;
    if (uDirection % 2 == 0) {
        res = uResolution.x;
    } else {
        res = uResolution.y;
    }
    float wrappedOffset = mod((uOffset - res * .5f) / uResolution.y, spacing);
    for (float s = -1.0 + wrappedOffset; s < 1.0; s += spacing)
    {
        vec2 c = dir * s + normal * mainThickness;
        col += line(p, c, c + hatchDir * len, thickness);
    }

    // directional gradient
    float gradCoord = dot(p, normal);
    float gradient = 1.0 - gradCoord / (len * 0.8);

    FragColor = col * gradient * uColor;
}