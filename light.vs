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
 float3 normal : NORMAL;
};

struct PIXEL_INPUT_TYPE
{
 float4 position : SV_POSITION;
 float2 tex : TEXCOORD0;
 float3 normal : NORMAL;
};

//============
//VertexShader
PIXEL_INPUT_TYPE Light_vertex_shader(VERTEX_INPUT_TYPE input)
{
 PIXEL_INPUT_TYPE output;

 input.position.w = 1.0f;

 output.position = mul(input.position, world_matrix);
 output.position = mul(output.position, view_matrix);
 output.position = mul(output.position, projection_matrix);
 output.tex = input.tex;

 //calc the normal vec against world
 output.normal = mul(input.normal, (float3x3)world_matrix);

 //normalize normal vector
 output.normal = normalize(output.normal);
 return output;
}