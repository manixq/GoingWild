#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_


//========
//includes
#include <d3d11.h>
#include <D3DX10math.h>
#include "texture_class.h"

//==========
//ModelClass
class ModelClass
{
public:
 ModelClass();
 ModelClass(const ModelClass&);
 ~ModelClass();

 bool Initialize(ID3D11Device*, WCHAR*);
 void Shutdown();
 void Render(ID3D11DeviceContext*);
 int Get_index_count();
 ID3D11ShaderResourceView* Get_texture();

private:
 struct VERTEX_TYPE
 {
  D3DXVECTOR3 position;
  D3DXVECTOR2 texture;
  D3DXVECTOR3 normal;
 };

 bool Initialize_buffers(ID3D11Device*);
 void Shutdown_buffers();
 void Render_buffers(ID3D11DeviceContext*);

 bool Load_texture(ID3D11Device*, WCHAR*);
 void Release_texture();

 ID3D11Buffer *vertex_buffer_, *index_buffer_;
 int vertex_count_, index_count_;

 TextureClass* texture_;
};
#endif