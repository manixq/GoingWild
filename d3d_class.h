#ifndef _D3D_CLASS_H_
#define _D3D_CLASS_H_

//=======
//linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//========
//includes
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

//===========
//D3DClass
class D3DClass
{
public:
 D3DClass();
 D3DClass(const D3DClass&);
 ~D3DClass();

 bool Initialize(int, int, bool, HWND, bool, float, float);
 void Shutdown();

 void Begin_scene(float, float, float, float);
 void End_scene();

 ID3D11Device* GetDevice();
 ID3D11DeviceContext* GetDeviceContext();

 void GetProjectionMatrix(D3DXMATRIX&);
 void GetWorldMatrix(D3DMATRIX&);
 void GetOrthoMatrix(D3DXMATRIX&);

 void GetVideoCardInfo(char*, int&);

private:
 bool vsync_enabled_;
 int video_card_memory_;
 char video_card_description_[128];
 IDXGISwapChain* swap_chain_;
 ID3D11Device* device_;
 ID3D11DeviceContext* device_context_;
 ID3D11RenderTargetView* render_target_view_;
 ID3D11Texture2D* depth_stencil_buffer_;
 ID3D11DepthStencilState* depth_stencil_state_;
 ID3D11DepthStencilView* depth_stencil_view_;
 ID3D11RasterizerState* raster_state_;
 D3DXMATRIX projection_matrix_;
 D3DXMATRIX world_matrix_;
 D3DXMATRIX ortho_matrix_;
};

#endif