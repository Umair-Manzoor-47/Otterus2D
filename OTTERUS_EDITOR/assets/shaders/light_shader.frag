#version 450 core
in vec2 fragUVs;
out vec4 color;

uniform vec4 uLightColor;
uniform float uIntensity;
uniform float uFalloff;

uniform bool uIsSpotlight;
uniform vec2 uDirection;
uniform float uInnerCos;
uniform float uOuterCos;

void main()
{
    vec2 offset = (fragUVs - vec2(0.5)) * 2.0;
    float dist = length(offset);

    if (dist >= 1.0)
        discard;

    float atten = pow(1.0 - dist, uFalloff);

    if (uIsSpotlight && dist > 0.0001)
    {
        // uDirection is expected to be normalized on the CPU; reuse dist instead of calling normalize(offset)
        float theta = dot(offset / dist, uDirection);
        float spot = clamp((theta - uOuterCos) / max(uInnerCos - uOuterCos, 0.0001), 0.0, 1.0);
        atten *= spot;
    }

    color = vec4(uLightColor.rgb * uLightColor.a * uIntensity * atten, 1.0);
}
