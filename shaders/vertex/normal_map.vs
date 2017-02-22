//=======
//GLOBALS
cbuffer matrix_buffer
{
 matrix world_matrix;
 matrix view_matrix;
 matrix projection_matrix;
};

cbuffer camera_buffer
{
float3 camera_position;
float padding;
};

//========
//TypeDefs
struct VERTEX_INPUT_TYPE
{
 float4 position : POSITION;
 float2 tex : TEXCOORD0;
 float3 normal : NORMAL;
 float3 tangent : TANGENT;
 float3 binormal : BINORMAL;
};

struct PIXEL_INPUT_TYPE
{
 float4 position : SV_POSITION;
 float2 tex : TEXCOORD0;
 float3 normal : NORMAL;
 float3 tangent : TANGENT;
 float3 binormal : BINORMAL;
 float3 view_direction : TEXCOORD1;
};

//============
//VertexShader
PIXEL_INPUT_TYPE Normal_vertex_shader(VERTEX_INPUT_TYPE input)
{
 PIXEL_INPUT_TYPE output;
 float4 world_position;

 input.position.w = 1.0f;

 output.position = mul(input.position, world_matrix);
 output.position = mul(output.position, view_matrix);
 output.position = mul(output.position, projection_matrix);
 output.tex = input.tex;

 //calc the normal vec against world
 output.normal = mul(input.normal, (float3x3)world_matrix);

 //normalize normal vector
 output.normal = normalize(output.normal);


 //calc vertex pos in the world
 world_position = mul(input.position, world_matrix);

 //viewing direction
 output.view_direction = camera_position.xyz - world_position.xyz;
 output.view_direction = normalize(output.view_direction);

 output.tangent = mul(input.tangent, (float3x3)world_matrix);
 output.tangent = normalize(output.tangent);

 output.binormal = mul(input.binormal, (float3x3)world_matrix);
 output.binormal = normalize(output.binormal);

 return output;
}