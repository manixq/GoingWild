#ifndef _TERRAIN_CLASS_H_
#define _TERRAIN_CLASS_H_

//========
//INCLUDES
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <stdio.h>

using namespace DirectX;
using namespace std;

//============
//TerrainClass
class TerrainClass
{
    struct VERTEX_TYPE
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
        XMFLOAT3 color;
    };

    struct HEIGHT_MAP_TYPE
    {
        float x, y, z;
        float nx, ny, nz;
        float r, g, b;
    };

    struct MODEL_TYPE
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
        float r, g, b;
    };

    struct VECTOR_TYPE
    {
        float x, y, z;
    };

public:
    TerrainClass();
    TerrainClass(const TerrainClass&);
    ~TerrainClass();

    bool Initialize(ID3D11Device*, char*);
    void Shutdown();
    bool Render(ID3D11DeviceContext*);

    int Get_index_count();

private:
    bool Load_setup_file(char*);
    bool Load_bitmap_height_map();
    void Shutdown_height_map();
    void Set_terrain_coordinates();
    bool Calculate_normals();
    bool Load_color_map();
    bool Build_terrain_model();
    void Shutdown_terrain_model();

    bool Initialize_buffer(ID3D11Device*);
    void Shutdown_buffers();
    void Render_buffers(ID3D11DeviceContext*);

    ID3D11Buffer *vertex_buffer_, *index_buffer_;
    int vertex_count_, index_count_;

    int terrain_height_, terrain_width_;
    float height_scale_;
    char *terrain_filename_, *color_map_filename_;
    HEIGHT_MAP_TYPE* height_map_;
    MODEL_TYPE* terrain_model_;
};
#endif
