#ifndef _SHADER_MANAGER_CLASS_H_
#define _SHADER_MANAGER_CLASS_H_

//========
//INCLUDES
#include "d3d_class.h"
#include "deferred_shader_class.h"
#include "depth_shader_class.h"
#include "fire_shader_class.h"
#include "font_shader_class.h"
#include "horizontal_blur_shader_class.h"
#include "light_shader_class.h"
#include "normal_shader_class.h"
#include "particle_shader_class.h"
#include "reflection_shader_class.h"
#include "refraction_shader_class.h"
#include "shadow_shader_class.h"
#include "skybox_shader_class.h"
#include "terrain_shader_class.h"
#include "texture_shader_class.h"
#include "vertical_blur_shader_class.h"
#include "water_shader_class.h"

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

    bool Render_deferred_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
    bool Render_depth_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
    bool Render_fire_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
        ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);
    bool Render_font_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
    bool Render_horizontal_blur_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);
    bool Render_light_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR3);
    bool Render_normal_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, 
        D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);
    bool Render_particle_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
    bool Render_reflection_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXMATRIX);
    bool Render_refraction_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4);
    bool Render_shadow_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
        D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
    bool Render_skybox_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float, float, XMFLOAT4, XMFLOAT4);
    bool Render_terrain_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
        ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
    bool Render_texture_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
    bool Render_vertical_blur_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);
    bool Render_water_shader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
        ID3D11ShaderResourceView*, float, float);

private:
    DeferredShaderClass* deferred_shader_;
    DepthShaderClass* depth_shader_;
    FireShaderClass* fire_shader_;
    FontShaderClass* font_shader_;
    HorizontalBlurShaderClass* horizontal_blur_shader_;
    LightShaderClass* light_shader_;
    NormalShaderClass* normal_shader_;
    ParticleShaderClass* particle_shader_;
    ReflectionShaderClass* reflection_shader_;
    RefractionShaderClass* refraction_shader_;
    ShadowShaderClass* shadow_shader_;
    SkyboxShaderClass* skybox_shader_;
    TerrainShaderClass* terrain_shader_;
    TextureShaderClass* texture_shader_;
    VerticalBlurShaderClass* vertical_blur_shader_;
    WaterShaderClass* water_shader_;
};
#endif