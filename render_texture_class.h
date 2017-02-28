#ifndef _RENDER_TEXTURE_CLASS_H_
#define _RENDER_TEXTURE_CLASS_H_

//========
//Includes
#include <d3d11.h>

//==================
//RenderTextureClass
class RenderTextureClass
{
public:
 RenderTextureClass();
 RenderTextureClass(const RenderTextureClass&);
 ~RenderTextureClass();

 bool Initialize(ID3D11Device*, int, int);
 void Shutdown();

 void Set_render_target(ID3D11DeviceContext*, ID3D11DepthStencilView*);
 void Clear_render_target(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
 ID3D11ShaderResourceView* Get_shader_resource_view();

private:
 ID3D11Texture2D* render_target_texture_;
 ID3D11RenderTargetView* render_target_view_;
 ID3D11ShaderResourceView* shader_resource_view_;
};
#endif
