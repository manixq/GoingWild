#ifndef _REFLECTION_SHADER_CLASS_H_
#define  _REFLECTION_SHADER_CLASS_H_

//========
//Includes
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

//=====================
//ReflectionShaderClass
class ReflectionShaderClass
{
public:
 ReflectionShaderClass();
 ReflectionShaderClass(const ReflectionShaderClass&);
 ~ReflectionShaderClass();

 bool Initialize(ID3D11Device*, HWND);
 void Shutdown();
 bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
  ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXMATRIX);

private:
 struct MATRIXBUFFERTYPE
 {
  D3DXMATRIX world;
  D3DXMATRIX view;
  D3DXMATRIX projection;
 };

 struct REFLECTIONBUFFERTYPE
 {
  D3DXMATRIX reflection_matrix;
 };

 bool Initialize_shader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
 void Shutdown_shader();
 void Output_shader_error_message(ID3D10Blob*, HWND, WCHAR*);
 bool Set_shader_parameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
  ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXMATRIX);
 void Render_shader(ID3D11DeviceContext*, int);
 
 ID3D11VertexShader* vertex_shader_;
 ID3D11PixelShader* pixel_shader_;
 ID3D11InputLayout* input_layout_;
 ID3D11Buffer* matrix_buffer_;
 ID3D11SamplerState* sample_state_;
 ID3D11Buffer* reflection_buffer_;
};
#endif
