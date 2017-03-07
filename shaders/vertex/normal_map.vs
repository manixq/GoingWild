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

cbuffer clip_plane_bufffer
{
 float4 clip_plane;
};

cbuffer reflection_buffer
{
 matrix reflection_matrix;
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
	float4 reflection_position : TEXCOORD2;
	float4 depth_position : TEXTURE0;
 //float clip : SV_ClipDistance0;
};

//============
//VertexShader
PIXEL_INPUT_TYPE Normal_vertex_shader(VERTEX_INPUT_TYPE input)
{
	PIXEL_INPUT_TYPE output;
	matrix reflect_project_world;
	float4 world_position;

	input.position.w = 1.0f;

	output.position = mul(input.position, world_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);
	output.tex = input.tex;
	output.depth_position = output.position;
	//calc the normal vec against world
	output.normal = mul(input.normal, (float3x3)world_matrix);

	reflect_project_world = mul(reflection_matrix, projection_matrix);
	reflect_project_world = mul(world_matrix, reflect_project_world);

	output.reflection_position = mul(input.position, reflect_project_world);

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

	//output.clip = dot(mul(input.position, world_matrix), clip_plane);

	return output;
}