#version 450 core
out vec4 color;
in vec2 fragUVs;
in vec4 fragColor;

uniform sampler2D uTexture;
void main()
{
	vec4 textureColor = texture(uTexture, fragUVs);
	color = textureColor * fragColor;
}