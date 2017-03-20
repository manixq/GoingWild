//=======
//GLOBALS
cbuffer matrix_buffer
{
    matrix world_matrix;
    matrix view_matrix;
    matrix projection_matrix;
};

//========
//TypeDefs
struct VERTEX_INPUT_TYPE
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PIXEL_INPUT_TYPE
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//============
//VertexShader
PIXEL_INPUT_TYPE Light_vertex_shader(VERTEX_INPUT_TYPE input)
{
    PIXEL_INPUT_TYPE output;
    float4 world_position;

    input.position.w = 1.0f;

    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);

    output.tex = input.tex;    


    return output;
}