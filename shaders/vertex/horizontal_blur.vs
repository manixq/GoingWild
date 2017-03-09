//============
//Globals
cbuffer matrix_buffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

cbuffer screen_size_buffer
{
	float screen_width;
	float3 padding;
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

	float2 tex_coord1 : TEXCOORD1;
	float2 tex_coord2 : TEXCOORD2;
	float2 tex_coord3 : TEXCOORD3;
	float2 tex_coord4 : TEXCOORD4;
	float2 tex_coord5 : TEXCOORD5;
	float2 tex_coord6 : TEXCOORD6;
	float2 tex_coord7 : TEXCOORD7;
	float2 tex_coord8 : TEXCOORD8;
	float2 tex_coord9 : TEXCOORD9;
};

//=============
//Vertex shader
PIXEL_INPUT_TYPE Horizontal_blur_vertex_shader(VERTEX_INPUT_TYPE input)
{
	PIXEL_INPUT_TYPE output;
	float texel_size;

	//for proper matrix calc
	input.position.w = 1.0f;

	//calc positions
	output.position = mul(input.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);

	//store the txt coordinates for the pixel shader
	output.tex = input.tex;

	texel_size = 1.0f / screen_width;

	output.tex_coord1 = input.tex + float2(texel_size * -4.0f, 0.0f);
	output.tex_coord2 = input.tex + float2(texel_size * -3.0f, 0.0f);
	output.tex_coord3 = input.tex + float2(texel_size * -2.0f, 0.0f);
	output.tex_coord4 = input.tex + float2(texel_size * -1.0f, 0.0f);
	output.tex_coord5 = input.tex + float2(texel_size * 0.0f, 0.0f);
	output.tex_coord6 = input.tex + float2(texel_size * 1.0f, 0.0f);
	output.tex_coord7 = input.tex + float2(texel_size * 2.0f, 0.0f);
	output.tex_coord8 = input.tex + float2(texel_size * 3.0f, 0.0f);
	output.tex_coord9 = input.tex + float2(texel_size * 4.0f, 0.0f);

    return output;
}