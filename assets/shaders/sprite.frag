#version 330 core

in vec2 vTex;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    vec4 texColor = texture(uTexture, vTex);

    if (texColor.a < 0.1)
            discard;

    FragColor = texture(uTexture, vTex);
}