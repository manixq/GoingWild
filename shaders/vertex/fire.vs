//=======
//Globals
cbuffer MatrixBuffer
{
 matrix world_matrix;
 matrix view_matrix;
 matrix projection_matrix;
};

cbuffer NoiseBuffer
{
 float frame_time;
 float3 scroll_speed;
 float3 scale;
 float padding;
};

//========
//Typedefs
struct VERTEX_INPUT_TYPE
{
 float4 position : POSITION;
 float2 tex : TEXCOORD0;
};

struct PIXEL_INPUT_TYPE
{
 float4 position : SV_POSITION;
 float2 tex : TEXCOORD0;
 float2 tex_coord1 : TEXCOORD1;
 float2 tex_coord2 : TEXCOORD2;
 float2 tex_coord3 : TEXCOORD3;
};

//=============
//Vertex Shader

PIXEL_INPUT_TYPE Fire_vertex_shader(VERTEX_INPUT_TYPE input)
{
 PIXEL_INPUT_TYPE output;

 //position vector to be 4units for proper matrix calc
 input.position.w = 1.0f;

 output.position = mul(input.position, world_matrix);
 output.position = mul(output.position, view_matrix);
 output.position = mul(output.position, projection_matrix);
 output.tex = input.tex;

 output.tex_coord1 = (input.tex * scale.x);
 output.tex_coord1.y = output.tex_coord1.y + (frame_time * scroll_speed.x);

 output.tex_coord2 = (input.tex * scale.y);
 output.tex_coord2.y = output.tex_coord2.y + (frame_time * scroll_speed.y);

 output.tex_coord3 = (input.tex * scale.z);
 output.tex_coord3.y = output.tex_coord3.y + (frame_time * scroll_speed.z);

 return output;
}