#version 450 core
in vec2 fragUVs;
out vec4 color;

uniform sampler2D uSceneTexture;
uniform sampler2D uLightTexture;

void main()
{
    vec4 sceneColor = texture(uSceneTexture, fragUVs);
    vec3 lightColor = texture(uLightTexture, fragUVs).rgb;

    color = vec4(sceneColor.rgb * lightColor, sceneColor.a);
}
