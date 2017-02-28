#include "render_texture_class.h"

RenderTextureClass::RenderTextureClass()
{
 render_target_texture_ = nullptr;
 render_target_view_ = nullptr;
 shader_resource_view_ = nullptr;
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass& other)
{
 
}

RenderTextureClass::~RenderTextureClass()
{
 
}

bool RenderTextureClass::Initialize(ID3D11Device* device, int texture_width, int texture_height)
{
 D3D11_TEXTURE2D_DESC texture_desc;
 HRESULT result;
 D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
 D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;

 ZeroMemory(&texture_desc, sizeof(texture_desc));

 texture_desc.Width = texture_width;
 texture_desc.Height = texture_height;
 texture_desc.MipLevels = 1;
 texture_desc.ArraySize = 1;
 texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
 texture_desc.SampleDesc.Count = 1;
 texture_desc.Usage = D3D11_USAGE_DEFAULT;
 texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
 texture_desc.CPUAccessFlags = 0;
 texture_desc.MiscFlags = 0;

 result = device->CreateTexture2D(&texture_desc, nullptr, &render_target_texture_);
 if (FAILED(result))
  return false;

 render_target_view_desc.Format = texture_desc.Format;
 render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
 render_target_view_desc.Texture2D.MipSlice = 0;

 result = device->CreateRenderTargetView(render_target_texture_, &render_target_view_desc, &render_target_view_);
 if (FAILED(result))
  return false;

 shader_resource_view_desc.Format = texture_desc.Format;
 shader_resource_view_desc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
 shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
 shader_resource_view_desc.Texture2D.MipLevels = 1;

 result = device->CreateShaderResourceView(render_target_texture_, &shader_resource_view_desc, &shader_resource_view_);
 if (FAILED(result))
  return false;

 return true;
}

void RenderTextureClass::Shutdown()
{
 if(shader_resource_view_)
 {
  shader_resource_view_->Release();
  shader_resource_view_ = nullptr;
 }

 if(render_target_view_)
 {
  render_target_view_->Release();
  render_target_view_ = nullptr;
 }

 if(render_target_texture_)
 {
  render_target_texture_->Release();
  render_target_texture_ = nullptr;
 }
}

void RenderTextureClass::Set_render_target(ID3D11DeviceContext* device_context, ID3D11DepthStencilView* depth_stencil_view)
{
 device_context->OMSetRenderTargets(1, &render_target_view_, depth_stencil_view);
}

void RenderTextureClass::Clear_render_target(ID3D11DeviceContext* device_context, ID3D11DepthStencilView* depth_stencil_view, float red, float green, float blue, float alpha)
{
 float color[4];

 color[0] = red;
 color[1] = green;
 color[2] = blue;
 color[3] = alpha;

 device_context->ClearRenderTargetView(render_target_view_, color);
 device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTextureClass::Get_shader_resource_view()
{
 return shader_resource_view_;
}

