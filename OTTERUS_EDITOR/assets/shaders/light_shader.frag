#version 450 core
in vec2 fragUVs;
in vec4 fragColor;
flat in float fragIntensity;
flat in float fragFalloff;
flat in vec2 fragDirection;
flat in float fragInnerCos;
flat in float fragOuterCos;
flat in float fragIsSpotlight;

out vec4 color;

void main()
{
    vec2 offset = (fragUVs - vec2(0.5)) * 2.0;
    float dist = length(offset);

    if (dist >= 1.0)
        discard;

    float atten = pow(1.0 - dist, fragFalloff);

    if (fragIsSpotlight > 0.5 && dist > 0.0001)
    {
        float theta = dot(offset / dist, fragDirection);
        float spot = clamp((theta - fragOuterCos) / max(fragInnerCos - fragOuterCos, 0.0001), 0.0, 1.0);
        atten *= spot;
    }

    color = vec4(fragColor.rgb * fragColor.a * fragIntensity * atten, 1.0);
}
