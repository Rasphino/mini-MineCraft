#version 330 core
out vec4 FragColor;
uniform vec3 cubePos;
uniform vec3 cameraPos;
in vec2 TexCoord;
uniform sampler2D high_grass;

void main()
{
    vec3 texColor = texture(high_grass, TexCoord).rgb;
    if(texColor.r < 0.00001 && texColor.b < 0.00001 && texColor.g < 0.00001)
        discard;
    float dist = length((cameraPos - cubePos).xyz);
    float maxDist = 50;
    float minDist = 20;
    vec3 fogcolor = vec3(0.78,0.78,0.82);
    float fogFactor = (maxDist - dist) / (maxDist - minDist);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );
    vec3 color = mix(fogcolor, texColor, fogFactor);
    FragColor = vec4(color, 1.0);
}
