#ifndef _FIRE_SHADER_CLASS_H_
#define _FIRE_SHADER_CLASS_H

//========
//Includes
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

//===============
//FireShaderClass
class FireShaderClass
{
public:
 FireShaderClass();
 FireShaderClass(const FireShaderClass&);
 ~FireShaderClass();

 bool Initialize(ID3D11Device*, HWND);
 void Shutdown();
 bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);

private:
 struct MATRIX_BUFFER_TYPE
 {
  D3DXMATRIX world;
  D3DXMATRIX view;
  D3DXMATRIX projection;
 };

 struct NOISE_BUFFER_TYPE
 {
  float frame_time;
  D3DXVECTOR3 scroll_speed;
  D3DXVECTOR3 scale;
  float padding;
 };

 struct DISTORTION_BUFFER_TYPE
 {
  D3DXVECTOR2 distortion1;
  D3DXVECTOR2 distortion2;
  D3DXVECTOR2 distortion3;
  float distortion_scale;
  float distortion_bias;
 };

 bool Initialize_shader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
 void Shutdown_shader();
 void Output_shader_error_message(ID3D10Blob*, HWND, WCHAR*);
 bool Set_shader_parameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);
 void Render_shader(ID3D11DeviceContext*, int);

 ID3D11VertexShader* vertex_shader_;
 ID3D11PixelShader* pixel_shader_;
 ID3D11InputLayout* layout_;
 ID3D11Buffer* matrix_buffer_;
 ID3D11Buffer* noise_buffer_;
 ID3D11SamplerState* sample_state_;
 ID3D11SamplerState* sample_state2_;
 ID3D11Buffer* distortion_buffer_;
};

#endif
