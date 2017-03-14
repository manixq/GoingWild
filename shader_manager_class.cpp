#include "shader_manager_class.h"

ShaderManagerClass::ShaderManagerClass()
{
    texture_shader_ = nullptr;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass&)
{
    
}

ShaderManagerClass::~ShaderManagerClass()
{
    
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;

    //Texture Shader
    texture_shader_ = new TextureShaderClass;
    if (!texture_shader_)
        return false;
    result = texture_shader_->Initialize(device, hwnd);
    if(!result)
    {
        MessageBox(hwnd, L"Could not initialize texture shader.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ShaderManagerClass::Shutdown()
{
    //Texture shader
    if(texture_shader_)
    {
        texture_shader_->Shutdown();
        delete texture_shader_;
        texture_shader_ = nullptr;
    }
}

bool ShaderManagerClass::Render_texture_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture)
{
    bool result;
    result = texture_shader_->Render(device, index_count, world, view, projection, texture);
    if (result)
        return false;
    return true;
}
