#version 330 core
out vec4 FragColor;
uniform vec3 cubePos;
uniform vec3 cameraPos;

in vec2 TexCoord;
uniform sampler2D soil;
void main() {
    float dist = length((cameraPos - cubePos).xyz);
    float maxDist = 50;
     float minDist = 20;
     vec3 fogcolor = vec3(0.78,0.78,0.82);
     float fogFactor = (maxDist - dist) / (maxDist - minDist);
     fogFactor = clamp( fogFactor, 0.0, 1.0 );
     vec3 texcolor = texture(soil, TexCoord).rgb;
     vec3 color = mix(fogcolor, texcolor, fogFactor);
     FragColor = vec4(color, 1.0);
 }
