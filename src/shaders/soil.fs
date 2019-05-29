#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D soil;
void main() {
    FragColor = texture(soil, TexCoord);
}

