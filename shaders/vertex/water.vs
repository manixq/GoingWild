
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ReflectionBuffer
{
	matrix reflectionMatrix;
};


struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
    float4 refractionPosition : TEXCOORD2;
};


PixelInputType Water_vertex_shader(VertexInputType input)
{
    PixelInputType output;
   	matrix reflectProjectWorld;
   	matrix viewProjectWorld;
	
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
   	output.tex = input.tex;

    reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
    reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

    output.reflectionPosition = mul(input.position, reflectProjectWorld);

    viewProjectWorld = mul(viewMatrix, projectionMatrix);
    viewProjectWorld = mul(worldMatrix, viewProjectWorld);

    output.refractionPosition = mul(input.position, viewProjectWorld);
    return output;
}