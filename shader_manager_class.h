#ifndef _SHADER_MANAGER_CLASS_H_
#define _SHADER_MANAGER_CLASS_H_

//========
//INCLUDES
#include "d3d_class.h"
#include "texture_shader_class.h"

//==================
//ShaderManagerClass
class ShaderManagerClass
{
public:
    ShaderManagerClass();
    ShaderManagerClass(const ShaderManagerClass&);
    ~ShaderManagerClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();

    bool Render_texture_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

private:
    TextureShaderClass* texture_shader_;
};
#endif