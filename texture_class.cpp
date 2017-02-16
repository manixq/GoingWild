#include "texture_class.h"

TextureClass::TextureClass()
{
 texture_ = nullptr;
}


TextureClass::TextureClass(const TextureClass&)
{
 
}


TextureClass::~TextureClass()
{
 
}


bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
 HRESULT result;

 result = D3DX11CreateShaderResourceViewFromFile(device, filename, nullptr, nullptr, &texture_, nullptr);
 if (FAILED(result))
  return false;

 return true;
}

void TextureClass::Shutdown()
{
 if(texture_)
 {
  texture_->Release();
  texture_ = nullptr;
 }
}

ID3D11ShaderResourceView* TextureClass::Get_texture()
{
 return texture_;
}








