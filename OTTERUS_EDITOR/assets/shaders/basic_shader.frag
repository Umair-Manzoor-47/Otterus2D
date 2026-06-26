#version 450 core
out vec4 color;
in vec2 fragUVs;
uniform sampler2D uTexture;
void main()
{
	color = texture(uTexture, fragUVs);
}