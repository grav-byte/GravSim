#version 330 core

in vec2 vTex;                 // uv
uniform float uTime;          // time in seconds
uniform float uTimeOffset;    // time offset in seconds
uniform sampler2D uNoiseTex;

out vec4 fragColor;

#define PI 3.1415926
#define ETIL 2
#define NUM_PARTICLE 30
#define GRAVITY 0.1
#define EXPLODE_POW 0.3
#define EXPLODE_STEP 0.5
#define NORMAL_FLICKER 0.005
#define CIRCLE_FLICKER 0.02
#define DD1 2.0
#define DD2 3.0
#define SPEED .5

//float Time = (mod(uTime, 3) - uTimeOffset);
float Time = (uTime - uTimeOffset);

float hash(float n)
{
    return fract(sin(n) * 43758.5453123);
}

vec2 noise(vec2 tc)
{
    // deterministic during a run, different between runs using uTimeOffset as seed
    vec2 uv = tc + vec2(uTimeOffset * 12.9898, uTimeOffset * 78.233);
    vec2 n = texture(uNoiseTex, fract(uv)).xy;
    return 2.0 * n - 1.0;
}

vec2 getPos(vec2 o, float t, vec2 d)
{
    return vec2(o.x + d.x * t, o.y + d.y * t - GRAVITY * t * t);
}

vec2 getPos2(vec2 o, float t, vec2 d)
{
    return o + d * t;
}

float drawPoint(float r, float size, vec2 p)
{
    return smoothstep(r, r + size, length(p));
}

vec4 drawParticle(vec2 p, float size, vec3 col)
{
    float d = dot(p, p) * 90.0;
    float a = 1.0 - smoothstep(0.0, size * 1.5, d);
    float energy = pow(1.0 - smoothstep(0.0, size, d), 0.5);
    float overbright = 2.0;
    return vec4(col * energy * overbright, a);
}

vec4 drawFly(vec2 uv, vec2 o, float off, vec3 color, vec2 initDir)
{
    float t = Time * 3.0 * SPEED + off;
    if (t < 0.0 || t > DD1) return vec4(0.0);

    float nt = off + o.x + o.y;
    vec2 ep = getPos(o, 0.0, initDir);
    float lerp = t / DD1;

    vec4 col = vec4(0.0);

    for (int i = 0; i < ETIL; i++)
    {
        float id = float(i) / float(ETIL);
        for (int j = 0; j < NUM_PARTICLE; j++)
        {
            vec2 dir = noise(vec2(
            float(j) / float(NUM_PARTICLE),
            hash(float(j) + nt) * 0.5)) * EXPLODE_POW;

            vec2 q = uv - getPos(ep, t - 0.2 * EXPLODE_STEP * id, dir);
            float flicker = NORMAL_FLICKER * hash(float(j) + lerp);

            vec4 pcol = drawParticle(
            q,
            (0.01 + flicker) * abs(cos(lerp * 0.5 * PI)),
            color * (1.0 - id));
            float glowStrength = 5.0;
            col.rgb += pcol.rgb * pcol.a * glowStrength;
            col.a += pcol.a;
        }
    }

    float a = 1.0 - drawPoint(0.0, 0.1, uv - ep);
    vec3 rgb = color * (1.0 - clamp(3.0 * lerp, 0.0, 1.0));
    float coreBoost = 6.0;
    col.rgb += rgb * a * coreBoost;
    col.a += a;
    col.rgb += color * a * 1.2;

    return col;
}

vec4 drawFlyCircle(vec2 uv, vec2 o, float off, vec3 color, vec2 initDir, float r)
{
    float t = Time * 5.0 * SPEED + off;
    if (t < 0.0 || t > DD2) return vec4(0.0);

    float nt = off + o.x + o.y;
    vec2 ep = getPos(o, 0.0, initDir);
    float lerp = t / DD2;

    vec4 col = vec4(0.0);

    for (int j = 0; j < 50; j++)
    {
        vec2 dir = normalize(noise(vec2(
        float(j) / 50.0,
        hash(float(j) + nt) * 0.5)));

        vec2 q = uv - getPos2(ep, t, r * dir);
        float flicker = CIRCLE_FLICKER * hash(float(j) + lerp);

        vec4 pcol = drawParticle(
        q,
        (0.01 + flicker) * abs(cos(lerp * 0.5 * PI)),
        color);
        float glowStrength = 4.0;
        col.rgb += pcol.rgb * pcol.a * glowStrength;
        col.a += pcol.a;
    }

    float a = 1.0 - drawPoint(0.0, 0.7, uv - ep);
    vec3 rgb = color * (1.0 - clamp(3.0 * lerp, 0.0, 1.0));
    float coreBoost = 5.0;
    col.rgb += rgb * a * coreBoost;
    col.a += a;
    col.rgb += color * a * 1.0;

    return col;
}

void main()
{
    vec2 uv = vTex * 2.0 - .5;

    vec4 col = vec4(0.0);
    col += drawFly(uv, vec2(0.5), 0.0, vec3(0.8, 0.4, 0.0), vec2(0.07, 0.36));
    col += drawFlyCircle(uv, vec2(0.5), 0.0, vec3(0.8, 0.3, 0.0),
    vec2(-0.1, 0.3), 0.05);

    float fade = clamp(1.0 - Time * 2.0 / DD1, 0.0, 1.0);
    fragColor = vec4(col.rgb, col.a * fade);
}