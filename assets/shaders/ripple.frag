#version 330 core

in vec2 vTex;          // UVs (0..1)

uniform float uTime;              // time in seconds
uniform sampler2D uScreenBuffer;    // input texture
uniform vec2 uResolution;           // framebuffer size in pixels

out vec4 fragColor;                // output color

void main()
{
    // Sawtooth function to pulse from centre
    float offset = fract(uTime) / uTime;
    float CurrentTime = uTime * offset;

    vec3 WaveParams = vec3(10.0, 0.8, 0.1);
    vec2 texCoord = vTex;
    vec2 coord = texCoord;
    float aspect = uResolution.x / uResolution.y;
    coord.x *= aspect;

    vec2 WaveCentre = vec2(0.5, 0.5);

    vec2 center = WaveCentre;
    center.x *= aspect;

    float Dist = distance(coord, center);

    vec4 Color = texture(uScreenBuffer, texCoord);

    if (Dist <= (CurrentTime + WaveParams.z) && Dist >= (CurrentTime - WaveParams.z))
    {
        float Diff = Dist - CurrentTime;
        float ScaleDiff = 1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y);
        float DiffTime = Diff * ScaleDiff;

        vec2 DiffTexCoord = normalize(coord - center);

        coord += (DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.0);

        vec2 sampleUV = coord;
        sampleUV.x /= aspect;

        Color = texture(uScreenBuffer, sampleUV);
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
    }

    fragColor = Color;
}