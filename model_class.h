#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_


//========
//includes
#include <d3d11.h>
#include <D3DX10math.h>
#include "texture_class.h"
#include <fstream>

using namespace std;

//==========
//ModelClass
class ModelClass
{
public:
 ModelClass();
 ModelClass(const ModelClass&);
 ~ModelClass();

 bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
 void Shutdown();
 void Render(ID3D11DeviceContext*);
 int Get_index_count();
 ID3D11ShaderResourceView** Get_texture();
 bool Load_model(char*);
 void Release_model();

private:
 struct VERTEX_TYPE
 {
  D3DXVECTOR3 position;
  D3DXVECTOR2 texture;
  D3DXVECTOR3 normal;
  D3DXVECTOR3 tangent;
  D3DXVECTOR3 binormal;
 };

 struct MODEL_TYPE
 {
  float x, y, z;
  float tu, tv;
  float nx, ny, nz;
  float tx, ty, tz;
  float bx, by, bz;
 };

 //struct for calc tangent and binormal
 struct TEMP_VERTEX_TYPE
 {
  float x, y, z;
  float tu, tv;
  float nx, ny, nz;
 };

 struct VECTOR_TYPE
 {
  float x, y, z;
 };

 void Calculate_model_vectors();
 void Calculate_tangent_binormal(TEMP_VERTEX_TYPE, TEMP_VERTEX_TYPE, TEMP_VERTEX_TYPE, VECTOR_TYPE&, VECTOR_TYPE&);
 void Calculate_normal(VECTOR_TYPE, VECTOR_TYPE, VECTOR_TYPE&);

 bool Initialize_buffers(ID3D11Device*);
 void Shutdown_buffers();
 void Render_buffers(ID3D11DeviceContext*);

 bool Load_texture(ID3D11Device*, WCHAR*, WCHAR*);
 void Release_texture();

 ID3D11Buffer *vertex_buffer_, *index_buffer_;
 int vertex_count_, index_count_;
 
 TextureClass* texture_;
 MODEL_TYPE* model_;
};
#endif