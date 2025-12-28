// 1D perlin noise by ikuto         | https://www.shadertoy.com/view/lt3BWM
// 2D vesica SDF by iq              | https://www.shadertoy.com/view/XtVfRW
// fire based on Trisomie21         | https://www.shadertoy.com/view/lsf3RH
// mach diamonds based on xingyzt   | https://www.shadertoy.com/view/WdGBDc
#version 330 core

#define HASHSCALE 0.1031
#define PI     3.1416

out vec4 FragColor;
in vec2 vTex;

uniform float uTime;
uniform float uThrust;

float angle = .01;
float feather = .1;
float base = -.1;

float hash(float p)
{
	vec3 p3  = fract(vec3(p) * HASHSCALE);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}
float hash31( vec3 p )
{
	float h = dot(p,vec3(17, 1527, 113));
    return fract(sin(h)*43758.5453123);
}


float fade(float t) { return t*t*t*(t*(6.*t-15.)+10.); }

float grad(float hash, float p)
{
    int i = int(1e4*hash);
	return (i & 1) == 0 ? p : -p;
}

float perlin(float p)
{
	float pi = floor(p), pf = p - pi, w = fade(pf);
    return mix(grad(hash(pi), pf), grad(hash(pi + 1.0), pf - 1.0), w) * 2.0;
}

float vesica_vertical(vec2 p, float r, float d)
{
    // rotate the vesica 90° for vertical orientation
    vec2 pv = vec2(p.y, p.x);  // swap axes for vertical
    pv = abs(pv);

    float b = sqrt(r*r - d*d);

    return ((pv.x - b) * d > pv.y * b) ? length(pv - vec2(b, 0.0)) * sign(d)
                                       : length(pv - vec2(0.0, -d)) - r;
}

float mvesica(vec2 p)
{
    return vesica_vertical(p, 1.0, 0.8);
}

float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float smoothsign( float s, float d )
{
    return smoothstep(-s,s,d)*2.-1.;
}

float shell(vec2 p, float offset)
{
    float main = mvesica(p);
    float next = min(
        mvesica(p + vec2(0, offset)),
        mvesica(p - vec2(0, offset))
    );
    return smin(main, next, 0.15);
}

vec4 mach(vec2 vTex, float t)
{
    float offset = 0.8;

    float top = vTex.y;
    float bottom = 1.0 - top;

    vec2 uv = vTex * 2.0 - 1.0;       // -1..1
    uv.y *= -1.0;                     // flip if necessary
    uv *= 2.0;

    // distortions
    uv.y += 0.03 * perlin(t * 0.5 + top) * (0.5 + top);
    uv.y += 0.01 * perlin(t * 7.0 + top) * (0.5 + top);
    uv.y += 0.01 * perlin(t * 67.0 + top) * (0.5 + top);
    uv.y += 0.01 * perlin(t * 101.0 + top) * (0.5 + top);

    uv.x += 0.03 * (0.2 - abs(uv.y)) * perlin(t * 3.0);

    vec2 p = uv;
    p.y = mod(p.y + offset/2.0, offset) - offset/2.0;  // vertical repetition

    float diamonds = max(smin(
        vesica_vertical(p - vec2(0, offset - 0.4), 0.3, 0.15),
        max(vesica_vertical(p + vec2(0, offset - 0.4), 0.85, 0.7), 0.0) + 0.05 * (1.0 - 2.0 * abs(p.y - 0.1)),
        0.1
    ), 0.0);

    float exhaust = shell(p, offset);

    float streams = 1.0;
    for(float i = 0.0; i < 6.0; i++) {
        if(perlin(i + t) > 0.2) continue;
        p.y += perlin(i - t) * 0.05;  // move vertically instead of horizontally
        streams *= (abs(shell(p + vec2(0, sign(p.y)*.005*i*i), offset)) < 0.005) ? 0.0 : 1.0;
    }

    float outside = max(sign(exhaust), 0.0);
    float inside = 1.0 - outside;

    float soutside = (1.0 + smoothsign(0.1, exhaust)) / 2.0;
    float sinside = 1.0 - soutside;

    float d = smin(exhaust, -diamonds, -0.03);
    d = abs(d);

    float lum = (
        1.0 - d *
        (
            2.0 * inside +
            5.0 * outside / (top + 0.4)
        )
    );
    lum = clamp(lum, 0.0, 1.0);
    lum = pow(lum, (0.9 * inside + 1.5 * outside));

    vec3 col = vec3(lum);

    col *= vec3(
        (1.05 * sinside + 1.40 * soutside - top * 0.0),
        (1.00 * sinside + 1.00 * soutside - top * 0.2),
        (1.40 * sinside + 1.80 * soutside - top * 0.6)
    );

    col -= diamonds;
    col *= 1.0; // -0.1 * streams * perlin(0.5 * uv.y - 5.0 * t + perlin(uv.x + uv.y + t));

    col = vTex.y < .75 ? col : vec3(0.0);
    float alpha = .8-vTex.y;
    alpha = 1.0;
    return vec4(col * alpha, alpha);
}

float snoise(vec3 uv, float res)
{
	const vec3 s = vec3(1e0, 1e2, 1e3);

	uv *= res;

	vec3 uv0 = floor(mod(uv, res))*s;
	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;

	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);

	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);

	vec4 r = fract(sin(v*1e-1)*1e3);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	return mix(r0, r1, f.z)*2.-1.;
}

void main()
{
    // vertical offset
    vec2 shaderOffset = vec2(0.0, 0.3); // adjust vertical position
    float scale = 2.0;                   // scale everything down

    // scale and center vTex
    vec2 centeredTex = (vTex - 0.5) * scale + 0.5;

    // compute position
    vec2 p = -vec2(.5, .8) + centeredTex + shaderOffset;

    float color = 3.0 - (3.0 * length(2.0 * p));

    vec3 coord = vec3(atan(p.x, p.y) / 6.2832 + 0.5, length(p) * 0.4, 0.5);

    // add layered noise
    for(int i = 1; i <= 7; i++)
    {
        float power = pow(2.0, float(i));
        color += (1.5 / power) * snoise(
        coord + vec3(0.0, -uTime * pow(uThrust, 0.2), uTime * 0.2 * uThrust),
        power * 16.0
        );
    }

    angle *= (1.0 - uThrust * 2.0);

    // compute mask
    float mask = 1.0 - smoothstep(angle, angle + feather, coord.x)
    * smoothstep(1.0 - angle, 1.0 - feather - angle, coord.x);
    mask = p.y < base ? mask : 0.0;
    mask *= max(0.0, uThrust - coord.y * 3.0 * uThrust) * 0.9 * (2.0 - uThrust);

    color *= mask;

    // base color
    vec3 col;
    col.r = color;
    col.g = pow(max(color, 0.0), 2.0) * (1.0 - 0.25 * uThrust);
    col.b = pow(max(color, 0.0), 3.0) * (0.15 + 0.65 * uThrust);

    FragColor = vec4(col, 1.0);

    // mach diamonds
    float t = uTime * uThrust * 4.0;
    vec4 machCol = mach(centeredTex, t);
    machCol.g = pow(max(machCol.g, 0.0), 2.0) * (1.0 - 0.25 * uThrust);
    machCol.b = pow(max(machCol.b, 0.0), 3.0) * (0.15 + 0.65 * uThrust);

    FragColor += machCol * uThrust * mask;

    // final alpha based on intensity
    FragColor.a = FragColor.r + FragColor.g + FragColor.b;
}