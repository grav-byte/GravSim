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
#define SPEED 1.0

float hash(float n)
{
    return fract(sin(n) * 43758.5453123);
}

vec2 noise(vec2 tc)
{
    return 2.0 * texture(uNoiseTex, tc).xy - 1.0;
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

vec3 drawParticle(vec2 p, float size, vec3 col)
{
    return mix(col, vec3(0.0),
    smoothstep(0.0, size, dot(p, p) * 90.0));
}

vec3 drawFly(vec2 uv, vec2 o, float off, vec3 color, vec2 initDir)
{
    float t = (uTime - uTimeOffset) * 3.0 * SPEED + off;
    if (t < 0.0 || t > DD1) return vec3(0.0);

    float nt = off + o.x + o.y;
    vec2 ep = getPos(o, 0.0, initDir);
    float lerp = t / DD1;

    vec3 col = vec3(0.0);

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

            col += drawParticle(
            q,
            (0.01 + flicker) * abs(cos(lerp * 0.5 * PI)),
            color * (1.0 - id));
        }
    }

    vec3 mix1 = mix(color, vec3(0.0), clamp(3.0 * lerp, 0.0, 1.0));
    col += mix(mix1, vec3(0.0), drawPoint(0.0, 0.1, uv - ep));

    return col;
}

vec3 drawFlyCircle(vec2 uv, vec2 o, float off, vec3 color, vec2 initDir, float r)
{
    float t = (uTime - uTimeOffset) * 5.0 * SPEED + off;
    if (t < 0.0 || t > DD2) return vec3(0.0);

    float nt = off + o.x + o.y;
    vec2 ep = getPos(o, 0.0, initDir);
    float lerp = t / DD2;

    vec3 col = vec3(0.0);

    for (int j = 0; j < 50; j++)
    {
        vec2 dir = normalize(noise(vec2(
        float(j) / 50.0,
        hash(float(j) + nt) * 0.5)));

        vec2 q = uv - getPos2(ep, t, r * dir);
        float flicker = CIRCLE_FLICKER * hash(float(j) + lerp);

        col += drawParticle(
        q,
        (0.01 + flicker) * abs(cos(lerp * 0.5 * PI)),
        color);
    }

    col += mix(
    mix(color, vec3(0.0), clamp(3.0 * lerp, 0.0, 1.0)),
    vec3(0.0),
    drawPoint(0.0, 0.7, uv - ep));

    return col;
}

void main()
{
    vec2 uv = vTex;

    vec3 col = vec3(0.0);
    col += drawFly(uv, vec2(0.5), 0.0, vec3(0.8, 0.5, 0.0), vec2(0.07, 0.36));
    col += drawFlyCircle(uv, vec2(0.5), 0.0, vec3(0.8, 0.8, 0.0),
    vec2(-0.1, 0.3), 0.05);

    // transparency
    float alpha = smoothstep(0.0, 0.15, max(max(col.r, col.g), col.b));
    fragColor = vec4(col, alpha);
}