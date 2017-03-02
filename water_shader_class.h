#ifndef _WATER_SHADER_CLASS_H_
#define _WATER_SHADER_CLASS_H_

//=======
//Include
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

//================
//WaterShaderClass
class WaterShaderClass
{
public:
 WaterShaderClass();
 WaterShaderClass(const WaterShaderClass&);
 ~WaterShaderClass();

 bool Initialize(ID3D11Device*, HWND);
 void Shutdown();
 bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float);

private:
 struct MATRIX_BUFFER_TYPE
 {
  D3DXMATRIX world;
  D3DXMATRIX view;
  D3DXMATRIX projection;
 };

 struct REFLECTION_BUFFER_TYPE
 {
  D3DXMATRIX reflection;
 };

 struct WATER_BUFFER_TYPE
 {
  float water_translation;
  float reflect_refract_scale;
  D3DXVECTOR2 padding;
 };

 bool Initialize_shader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
 void ShutdownShader();
 void Output_shader_error_message(ID3D10Blob*, HWND, WCHAR*);
 bool Set_shader_parameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float);
 void Render_shader(ID3D11DeviceContext*, int);

 ID3D11VertexShader* vertex_shader_;
 ID3D11PixelShader* pixel_shader_;
 ID3D11InputLayout* layout_;
 ID3D11SamplerState* sampler_state_;
 ID3D11Buffer* matrix_buffer_;

 ID3D11Buffer* reflectiion_buffer_;
 ID3D11Buffer* water_buffer_;
};

#endif
