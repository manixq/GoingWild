#ifndef _HORIZONTAL_BLUR_SHADER_CLASS_H_
#define _HORIZONTAL_BLUR_SHADER_CLASS_H_

//========
//Includes
#include <D3DX11.h>
#include <D3DX10math.h>
#include <d3dx10async.h>
#include <fstream>

using namespace std;

//=========================
//HorizontalBlurShaderClass
class HorizontalBlurShaderClass
{
public:
    HorizontalBlurShaderClass();
    HorizontalBlurShaderClass(const HorizontalBlurShaderClass&);
    ~HorizontalBlurShaderClass();
    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);

private:
    struct MATRIX_BUFFER_TYPE
    {
        D3DXMATRIX world;
        D3DXMATRIX view;
        D3DXMATRIX projection;
    };

    struct SCREEN_SIZE_BUFFER_TYPE
    {
        float screen_width;
        D3DXVECTOR3 padding;
    };

    bool Initialize_shader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void Shutdown_shader();
    void Output_shader_error_message(ID3D10Blob*, HWND, WCHAR*);

    bool Set_shader_parameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);
    void Render_shader(ID3D11DeviceContext*, int);

    ID3D11VertexShader* vertex_shader_;
    ID3D11PixelShader* pixel_shader_;
    ID3D11InputLayout* layout_;
    ID3D11Buffer* matrix_buffer_;
    ID3D11SamplerState* sample_state_;
    ID3D11Buffer* screen_size_buffer_;
};
#endif