#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D high_grass;

void main()
{     
	vec4 texColor = texture(high_grass, TexCoord);
    if(texColor.r < 0.00001 && texColor.b < 0.00001 && texColor.g < 0.00001)
        discard;
    FragColor = texColor;
}
