#version 330 core

in vec2 vTex;

uniform sampler2D uScreenBuffer;
uniform vec2 uResolution;

out vec4 fragColor;

void main()
{
    vec2 texel = 1.0 / uResolution;

    vec3 base = texture(uScreenBuffer, vTex).rgb;

    // ---- bright-pass: only keep pixels above threshold ----
    float lum = dot(base, vec3(0.2126, 0.7152, 0.0722));
    float thresholdLow = 0.6;
    float thresholdHigh = 1.4;
    vec3 bright = base * smoothstep(thresholdLow, thresholdHigh, lum);

    // ---- simple wide blur (9-tap cross) ----
    vec3 bloom = vec3(0.0);
    for(int x = -2; x <= 2; x++)
    {
        for(int y = -2; y <= 2; y++)
        {
            vec2 offset = texel * vec2(x, y);
            bloom += texture(uScreenBuffer, vTex + offset).rgb * smoothstep(thresholdLow, thresholdHigh,
                        dot(texture(uScreenBuffer, vTex + offset).rgb, vec3(0.2126,0.7152,0.0722)));
        }
    }
    bloom /= 25.0; // normalize 5x5 kernel

    // scale bloom intensity for HDR particles
    bloom *= 1.2;

    fragColor = vec4(base + bloom, 1.0);
}