#include "texture_class.h"

TextureClass::TextureClass()
{
 texture_[0] = nullptr;
 texture_[1] = nullptr;
}


TextureClass::TextureClass(const TextureClass& other)
{
 
}


TextureClass::~TextureClass()
{
 
}


bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename_1, WCHAR* filename_2)
{
 HRESULT result;

 result = D3DX11CreateShaderResourceViewFromFile(device, filename_1, nullptr, nullptr, &texture_[0], nullptr);
 if (FAILED(result))
  return false;
 result = D3DX11CreateShaderResourceViewFromFile(device, filename_2, nullptr, nullptr, &texture_[1], nullptr);
 if (FAILED(result))
  return false;
 return true;
}

void TextureClass::Shutdown()
{
 if(texture_[0])
 {
  texture_[0]->Release();
  texture_[0] = nullptr;
 }
 if (texture_[1])
 {
  texture_[1]->Release();
  texture_[1] = nullptr;
 }
}

ID3D11ShaderResourceView** TextureClass::Get_texture()
{
 return texture_;
}








