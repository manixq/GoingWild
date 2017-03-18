#ifndef _TERRAIN_CLASS_H_
#define _TERRAIN_CLASS_H_

//========
//INCLUDES
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

//============
//TerrainClass
class TerrainClass
{
    struct VERTEX_TYPE
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    TerrainClass();
    TerrainClass(const TerrainClass&);
    ~TerrainClass();

    bool Initialize(ID3D11Device*);
    void Shutdown();
    bool Render(ID3D11DeviceContext*);

    int Get_index_count();

private:
    bool Initialize_buffer(ID3D11Device*);
    void Shutdown_buffers();
    void Render_buffers(ID3D11DeviceContext*);

    ID3D11Buffer *vertex_buffer_, *index_buffer_;
    int vertex_count_, index_count_;
};
#endif
