#version 330 core

in vec2 vTex;          // UVs (0..1)
out vec4 FragColor;

uniform vec2 uResolution;
uniform vec4 uColor;       // tint
uniform float uRadius;

float line(vec2 p, vec2 a, vec2 b, float w)
{
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return smoothstep(w, 0.0, length(pa - ba * h));
}

void main()
{
    vec2 fragCoord = vTex * uResolution;
    vec2 p = (fragCoord - 0.5 * uResolution.xy) / uResolution.y;

    vec4 col = vec4(0.0f);

    float mainThickness = 0.002;

    // main circular boundary
    float d = abs(length(p) - uRadius);
    col += smoothstep(mainThickness, 0.0, d);

    // hatch pattern
    float spacing = radians(5.0); // angular spacing
    float hatchAngle = radians(45.0);
    float hatchLength = 0.05 * uRadius * 10.0;
    float hatchThickness = 0.001;

    for (float a = 0.0; a < 6.2; a += spacing)
    {
        vec2 radial = vec2(cos(a), sin(a));
        vec2 tangent = vec2(-radial.y, radial.x);
        vec2 hdir = normalize(radial + tangent);

        vec2 start = radial * uRadius;
        vec2 end   = start + hdir * hatchLength;

        col += line(p, start, end, hatchThickness);
    }

    vec4 gradient = vec4(1.0 - (length(p) - uRadius) * 22.0);

    FragColor = gradient * col * uColor;
}