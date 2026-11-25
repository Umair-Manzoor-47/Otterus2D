#version 450 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aIntensity;
layout (location = 4) in float aFalloff;
layout (location = 5) in vec2 aDirection;
layout (location = 6) in float aInnerCos;
layout (location = 7) in float aOuterCos;
layout (location = 8) in float aIsSpotlight;

out vec2 fragUVs;
out vec4 fragColor;
flat out float fragIntensity;
flat out float fragFalloff;
flat out vec2 fragDirection;
flat out float fragInnerCos;
flat out float fragOuterCos;
flat out float fragIsSpotlight;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
    fragUVs = aTexCoords;
    fragColor = aColor;
    fragIntensity = aIntensity;
    fragFalloff = aFalloff;
    fragDirection = aDirection;
    fragInnerCos = aInnerCos;
    fragOuterCos = aOuterCos;
    fragIsSpotlight = aIsSpotlight;
}
