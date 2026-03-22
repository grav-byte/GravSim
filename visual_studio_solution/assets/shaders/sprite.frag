#version 330 core

in vec2 vTex;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor; // tint color

void main() {
    vec4 texColor = texture(uTexture, vTex);

    // discard transparent pixels
    if (texColor.a < 0.1)
    discard;

    // multiply texture by tint color
    FragColor = texColor * uColor;
}