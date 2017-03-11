//=======
//Globals
cbuffer TesselationBuffer
{
    float tesselation_amount;
    float3 padding;
};


//========
//Typedefs
struct HULL_INPUT_TYPE
{
 float3 position : POSITION;
 float4 color : COLOR;
};

struct CONSTANT_OUTPUT_TYPE
{
 float edges[3] : SV_TessFactor;
 float inside : SV_InsideTessFactor;
};

struct HULL_OUTPUT_TYPE
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//=======================
//PATCH CONSTANT FUNCTION
CONSTANT_OUTPUT_TYPE Color_patch_constant_function(InputPatch<HULL_INPUT_TYPE, 3> input_patch, uint patch_id : SV_PrimitiveID)
{
    CONSTANT_OUTPUT_TYPE output;

    output.edges[0] = tesselation_amount;
    output.edges[1] = tesselation_amount;
    output.edges[2] = tesselation_amount;

    output.inside = tesselation_amount;

    return output;
}

//============================
//HULL SHADER
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("Color_patch_constant_function")]

HULL_OUTPUT_TYPE Color_hull_shader(InputPatch<HULL_INPUT_TYPE, 3> patch, uint point_id : SV_OutputControlPointID, uint patch_id : SV_PrimitiveID)
{
    HULL_OUTPUT_TYPE output;

    output.position = patch[point_id].position;
    output.color = patch[point_id].color;
    return output;
}