#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_

//========
//Includes
#include <d3d11.h>
#include <D3DX11tex.h>

//============
//TextureClass
class TextureClass
{
public:
 TextureClass();
 TextureClass(const TextureClass&);
 ~TextureClass();

 bool Initialize(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
 void Shutdown();
 ID3D11ShaderResourceView** Get_texture();

private:
 ID3D11ShaderResourceView* texture_[3];
};

#endif