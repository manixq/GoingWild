#include "shader_manager_class.h"

ShaderManagerClass::ShaderManagerClass()
{
    deferred_shader_ = nullptr;
    depth_shader_ = nullptr;
    fire_shader_ = nullptr;
    font_shader_ = nullptr;
    horizontal_blur_shader_ = nullptr;
    light_shader_ = nullptr;
    normal_shader_ = nullptr;
    particle_shader_ = nullptr;
    reflection_shader_ = nullptr;
    shadow_shader_ = nullptr;
    skybox_shader_ = nullptr;
    terrain_shader_ = nullptr;
    texture_shader_ = nullptr;
    vertical_blur_shader_ = nullptr;
    water_shader_ = nullptr;
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

    //Deferred Shader
    deferred_shader_ = new DeferredShaderClass;
    if (!deferred_shader_)
        return false;
    result = deferred_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize deferred shader.", L"Error", MB_OK);
        return false;
    }

    //Depth Shader
    depth_shader_ = new DepthShaderClass;
    if (!depth_shader_)
        return false;
    result = depth_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize depth shader.", L"Error", MB_OK);
        return false;
    }

    //Fire Shader
    fire_shader_ = new FireShaderClass;
    if (!fire_shader_)
        return false;
    result = fire_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize fire shader.", L"Error", MB_OK);
        return false;
    }

    //Font Shader
    font_shader_ = new FontShaderClass;
    if (!font_shader_)
        return false;
    result = font_shader_->Initialize(device, hwnd);
    if(!result)
    {
        MessageBox(hwnd, L"Could not initialize font shader.", L"Error", MB_OK);
        return false;
    }

    //Horizontal Blur shader
    horizontal_blur_shader_ = new HorizontalBlurShaderClass;
    if (!horizontal_blur_shader_)
        return false;
    result = horizontal_blur_shader_->Initialize(device, hwnd);
    if(!result)
    {
        MessageBox(hwnd, L"Could not initialize horizontal blur shader.", L"Error", MB_OK);
        return false;
    }

    //Light Shader
    light_shader_ = new LightShaderClass;
    if (!light_shader_)
        return false;
    result = light_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize light shader.", L"Error", MB_OK);
        return false;
    }

    //Normal Shader
    normal_shader_ = new NormalShaderClass;
    if (!normal_shader_)
        return false;
    result = normal_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize normal shader.", L"Error", MB_OK);
        return false;
    }

    //Particle Shader
    particle_shader_ = new ParticleShaderClass;
    if (!particle_shader_)
        return false;
    result = particle_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize particle shader.", L"Error", MB_OK);
        return false;
    }

    //Reflection Shader
    reflection_shader_ = new ReflectionShaderClass;
    if (!reflection_shader_)
        return false;
    result = reflection_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize reflection shader.", L"Error", MB_OK);
        return false;
    }

    //Refraction Shader
    refraction_shader_ = new RefractionShaderClass;
    if (!refraction_shader_)
        return false;
    result = refraction_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize refraction shader.", L"Error", MB_OK);
        return false;
    }

    //Shadow Shader
    shadow_shader_ = new ShadowShaderClass;
    if (!shadow_shader_)
        return false;
    result = shadow_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize shadow shader.", L"Error", MB_OK);
        return false;
    }

    //Skybox Shader
    skybox_shader_ = new SkyboxShaderClass;
    if (!skybox_shader_)
        return false;
    result = skybox_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize skybox shader.", L"Error", MB_OK);
        return false;
    }

    //Terrain Shader
    terrain_shader_ = new TerrainShaderClass;
    if (!terrain_shader_)
        return false;
    result = terrain_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize terrain shader.", L"Error", MB_OK);
        return false;
    }

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

    //Vertical Blur Shader
    vertical_blur_shader_ = new VerticalBlurShaderClass;
    if (!vertical_blur_shader_)
        return false;
    result = vertical_blur_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize vertical blur shader.", L"Error", MB_OK);
        return false;
    }

    //Water Shader
    water_shader_ = new WaterShaderClass;
    if (!water_shader_)
        return false;
    result = water_shader_->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize water shader.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ShaderManagerClass::Shutdown()
{
    if(deferred_shader_)
    {
        deferred_shader_->Shutdown();
        delete deferred_shader_;
        deferred_shader_ = nullptr;
    }

    if(depth_shader_)
    {
        depth_shader_->Shutdown();
        delete depth_shader_;
        depth_shader_ = nullptr;
    }

    if(fire_shader_)
    {
        fire_shader_->Shutdown();
        delete fire_shader_;
        fire_shader_ = nullptr;
    }

    if(font_shader_)
    {
        font_shader_->Shutdown();
        delete font_shader_;
        font_shader_ = nullptr;
    }

    if(horizontal_blur_shader_)
    {
        horizontal_blur_shader_->Shutdown();
        delete horizontal_blur_shader_;
        horizontal_blur_shader_ = nullptr;
    }

    if(light_shader_)
    {
        light_shader_->Shutdown();
        delete light_shader_;
        light_shader_ = nullptr;
    }

    if(normal_shader_)
    {
        normal_shader_->Shutdown();
        delete normal_shader_;
        normal_shader_ = nullptr;
    }

    if(particle_shader_)
    {
        particle_shader_->Shutdown();
        delete particle_shader_;
        particle_shader_ = nullptr;
    }

    if(reflection_shader_)
    {
        reflection_shader_->Shutdown();
        delete reflection_shader_;
        reflection_shader_ = nullptr;
    }

    if(refraction_shader_)
    {
        refraction_shader_->Shutdown();
        delete refraction_shader_;
        refraction_shader_ = nullptr;
    }

    if(shadow_shader_)
    {
        shadow_shader_->Shutdown();
        delete shadow_shader_;
        shadow_shader_ = nullptr;
    }

    if(skybox_shader_)
    {
        skybox_shader_->Shutdown();
        delete skybox_shader_;
        skybox_shader_ = nullptr;
    }

    if(terrain_shader_)
    {
        terrain_shader_->Shutdown();
        delete terrain_shader_;
        terrain_shader_ = nullptr;
    }

    if(texture_shader_)
    {
        texture_shader_->Shutdown();
        delete texture_shader_;
        texture_shader_ = nullptr;
    }

    if(vertical_blur_shader_)
    {
        vertical_blur_shader_->Shutdown();
        delete vertical_blur_shader_;
        vertical_blur_shader_ = nullptr;
    }

    if(water_shader_)
    {
        water_shader_->Shutdown();
        delete water_shader_;
        water_shader_ = nullptr;
    }
}

bool ShaderManagerClass::Render_deferred_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, 
    ID3D11ShaderResourceView* color_texture, ID3D11ShaderResourceView* normal_texture)
{
    bool result;
    result = deferred_shader_->Render(device, index_count, world, view, projection, color_texture, normal_texture);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_depth_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection)
{
    bool result;
    result = depth_shader_->Render(device, index_count, world, view, projection);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_fire_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* fire_texture, ID3D11ShaderResourceView* noise_texture, ID3D11ShaderResourceView* alpha_texture, float frame_time,
    D3DXVECTOR3 scroll_speed, D3DXVECTOR3 scale, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3, float distortion_scale, float distortion_bias)
{
    bool result;
    result = fire_shader_->Render(device, index_count, world, view, projection, fire_texture, noise_texture, alpha_texture, frame_time, scroll_speed,
        scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_font_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, 
    ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixel_color)
{
    bool result;
    result = font_shader_->Render(device, index_count, world, view, projection, texture, pixel_color);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_horizontal_blur_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* texture, float screen_width)
{
    bool result;
    result = horizontal_blur_shader_->Render(device, index_count, world, view, projection, texture, screen_width);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_light_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* color_texture, ID3D11ShaderResourceView* normal_texture, D3DXVECTOR3 light_direction)
{
    bool result;
    result = light_shader_->Render(device, index_count, world, view, projection, color_texture, normal_texture, light_direction);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_normal_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView** texture, D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color, D3DXVECTOR3 camera_position,
    D3DXVECTOR4 specular_color, float specular_power)
{
    bool result;
    result = normal_shader_->Render(device, index_count, world, view, projection, texture, light_direction, ambient_color, diffuse_color, camera_position,
        specular_color, specular_power);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_particle_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* texture)
{
    bool result;
    result = particle_shader_->Render(device, index_count, world, view, projection, texture);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_reflection_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view,D3DXMATRIX projection,
    ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* reflection_texture, D3DXMATRIX reflection_matrix)
{
    bool result;
    result = reflection_shader_->Render(device, index_count, world, view, projection, texture, reflection_texture, reflection_matrix);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_refraction_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* texture, D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color, D3DXVECTOR4 clip_plane)
{
    bool result = refraction_shader_->Render(device, index_count, world, view, projection, texture, light_direction, ambient_color, diffuse_color, clip_plane);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_shadow_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    D3DXMATRIX light_view, D3DXMATRIX light_projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depth_map_texture,
    D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color)
{
    bool result;
    result = shadow_shader_->Render(device, index_count, world, view, projection, light_view, light_projection, texture, depth_map_texture, light_direction,
        ambient_color, diffuse_color);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_skybox_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* perturb, ID3D11ShaderResourceView* clouds, float translation, float scale, float brightness, XMFLOAT4 apex_color, XMFLOAT4 center_color)
{
    bool result;
    result = skybox_shader_->Render(device, index_count, world, view, projection, perturb, clouds, translation, scale, brightness, apex_color, center_color);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_terrain_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    D3DXMATRIX light_view, D3DXMATRIX light_projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depth_map_texture, ID3D11ShaderResourceView* normal_map,
    ID3D11ShaderResourceView* normal_map2, D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color)
{
    bool result;
    result = terrain_shader_->Render(device, index_count, world, view, projection, light_view, light_projection, texture, depth_map_texture, normal_map, normal_map2, light_direction,
        ambient_color, diffuse_color);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_texture_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, 
    ID3D11ShaderResourceView* texture, float mouse_x, float mouse_y)
{
    bool result;
    result = texture_shader_->Render(device, index_count, world, view, projection, texture, mouse_x, mouse_y);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_vertical_blur_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection,
    ID3D11ShaderResourceView* texture, float screen_height)
{
    bool result;
    result = vertical_blur_shader_->Render(device, index_count, world, view, projection, texture, screen_height);
    if (!result)
        return false;
    return true;
}

bool ShaderManagerClass::Render_water_shader(ID3D11DeviceContext* device, int index_count, D3DXMATRIX world_matrix, D3DXMATRIX view_matrix,
    D3DXMATRIX projection_matrix, D3DXMATRIX reflection_matrix, ID3D11ShaderResourceView* reflection_texture, ID3D11ShaderResourceView* refraction_texture,
    ID3D11ShaderResourceView* normal_texture, float water_translation, float reflect_refract_scale)
{
    bool result;
    result = water_shader_->Render(device, index_count, world_matrix, view_matrix, projection_matrix, reflection_matrix, reflection_texture, refraction_texture,
        normal_texture, water_translation, reflect_refract_scale);
    if (!result)
        return false;
    return true;
}
