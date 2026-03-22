#version 330 core

in vec2 vTex;          // UVs (0..1)

uniform float uTime;              // time in seconds
uniform float uCenterX = 0.5;
uniform float uCenterY = 0.5;
uniform float uDirection = 1.0;
uniform float uRadius = 1.0;
uniform sampler2D uScreenBuffer;    // input texture
uniform vec2 uResolution;           // framebuffer size in pixels

out vec4 fragColor;                // output color

void main()
{
    // Parameters
    float MaxRadius = uRadius * .06;      // controls how far the wave travels
    float radiusNorm = 2.5 / max(MaxRadius, 0.0001);
    float WaveWidth = 0.05;     // thickness of the wave
    int NumWaves = 3;
    float WaveSpeed = max(1. * MaxRadius, .15) * uDirection;      // how fast it expands
    vec3 WaveParams = vec3(15.0, 0.2 * MaxRadius, 0.2 ); // distortion parameters


    float aspect = uResolution.x / uResolution.y;

    vec2 uv = vTex;
    vec2 p = uv;
    p.x *= aspect;

    vec2 WaveCentre = vec2(uCenterX, uCenterY);
    WaveCentre.x *= aspect;

    float Dist = distance(p, WaveCentre);

    // Compute current wave radius in a uniform way
    float CurrentRadius = mod(uTime * WaveSpeed, MaxRadius);

    // Sample the original color
    vec4 Color = texture(uScreenBuffer, vTex);

    for (int i = 0; i < NumWaves; i++)
    {
        float phase = float(i) / float(NumWaves);
        float waveRadius = mod(uTime * WaveSpeed + phase * MaxRadius, MaxRadius);

        if (abs(Dist - waveRadius) <= WaveWidth)
        {
            float Diff = Dist - waveRadius;
            float ScaleDiff = 1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y);
            float DiffTime = Diff * ScaleDiff;

            vec2 dir = normalize(p - WaveCentre);
            vec2 distortedUV = vTex + dir * DiffTime * radiusNorm;

            Color = texture(uScreenBuffer, distortedUV);
            Color += Color * ScaleDiff * radiusNorm * 1.5;
            Color.a = 1.0 - waveRadius / MaxRadius;
        }
    }

    fragColor = Color;
}