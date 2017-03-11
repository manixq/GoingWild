//========
//Typedefs

struct VERTEX_INPUT_TYPE
{
     float3 position : POSITION;
     float4 color : COLOR;
};

struct HULL_INPUT_TYPE
{
     float3 position : POSITION;
     float4 color : COLOR;
};


//=============
//Vertex Shader

HULL_INPUT_TYPE Color_vertex_shader(VERTEX_INPUT_TYPE input)
{
    HULL_INPUT_TYPE output;

    output.position = input.position;

    //store the input color for the pixel shader to use
    output.color = input.color;
    
    return output;
}