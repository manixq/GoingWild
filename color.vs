//=======
//Globals

cbuffer MatrixBuffer
{
 matrix world_matrix;
 matrix view_matrix;
 matrix projection_matrix;
};


//========
//Typedefs

struct VERTEX_INPUT_TYPE
{
 float4 position : POSITION;
 float4 color : COLOR;
};

struct PIXEL_INPUT_TYPE
{
 float4 position : SV_POSITION;
 float4 color : COLOR;
};


//=============
//Vertex Shader

PIXEL_INPUT_TYPE Color_vertex_shader(VERTEX_INPUT_TYPE input)
{
 PIXEL_INPUT_TYPE output;

 //position vector to be 4units for proper matrix calc
 input.position.w = 1.0f;

 //calc pos of the vert against the world, view, projection matrices
 output.position = mul(input.position, world_matrix);
 output.position = mul(output.position, view_matrix);
 output.position = mul(output.position, projection_matrix);

 //store the input color for the pixel shader to use
 output.color = input.color;

 return output;
}