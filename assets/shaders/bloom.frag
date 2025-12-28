#version 330 core

in vec2 vTex;

uniform sampler2D uScreenBuffer;
uniform vec2 uResolution;

out vec4 fragColor;

void main()
{
    vec2 texel = 1.0 / uResolution;

    vec3 base = texture(uScreenBuffer, vTex).rgb;

    // luminance
    float lum = dot(base, vec3(0.2126, 0.7152, 0.0722));

    // soft threshold instead of hard cutoff
    float bloomMask = smoothstep(.5, 1.4, lum);

    // ---- wider, softer blur ----
    vec3 blur = vec3(0.0);

    blur += texture(uScreenBuffer, vTex + texel * vec2( 1, 0)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2(-1, 0)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2( 0, 1)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2( 0,-1)).rgb;

    blur += texture(uScreenBuffer, vTex + texel * vec2( 2, 0)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2(-2, 0)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2( 0, 2)).rgb;
    blur += texture(uScreenBuffer, vTex + texel * vec2( 0,-2)).rgb;

    blur /= 8.0;

    // apply bloom mask + reduce intensity
    vec3 bloom = blur * bloomMask * .15;

    fragColor = vec4(base + bloom, 1.0);
}