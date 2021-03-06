#include "graphics_class.h"
#include "water_shader_class.h"

GraphicsClass::GraphicsClass()
{
    d3d_ = nullptr;
    camera_ = nullptr;
    frustum_ = nullptr;
    light_ = nullptr;
    model_list_ = nullptr;
    debug_window_ = nullptr;
    particle_system_ = nullptr;

    model_ = nullptr;
    ground_model_ = nullptr;
    wall_model_ = nullptr;
    bath_model_ = nullptr;
    water_model_ = nullptr;
    fire_model_ = nullptr;
    floor_model_ = nullptr;
    skybox_ = nullptr;

    reflection_texture_ = nullptr;
    refraction_texture_ = nullptr;
    render_texture_ = nullptr;
    down_sample_texture_ = nullptr;
    horizontal_blur_texture_ = nullptr;
    vertical_blur_texture_ = nullptr;
    up_sample_texture_ = nullptr;

    deferred_buffers_ = nullptr;

    small_window_ = nullptr;
    full_sceen_window_ = nullptr;

    mouse_ = nullptr;
    mouse_delta_x_ = 0;
    mouse_delta_y_ = 0;

    text_ = nullptr;
    texture_manager_ = nullptr;
    shader_manager_ = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screen_width, int screen_height, HWND hwnd)
{
    bool result;
    D3DXMATRIX static_view_matrix;
    int down_sample_width, down_sample_height;

    screen_height_ = screen_height;
    screen_width_ = screen_width;

    down_sample_width = screen_width / 2;
    down_sample_height = screen_height / 2;

    //create d3d object
    d3d_ = new D3DClass;
    if (!d3d_)
        return false;

    //init d3d object
    result = d3d_->Initialize(
        screen_width,
        screen_height,
        VSYNC_ENABLED,
        hwnd,
        FULL_SCREEN,
        SCREEN_DEPTH,
        SCREEN_NEAR);

    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    frustum_ = new FrustumClass;
    if (!frustum_)
        return false;

    shader_manager_ = new ShaderManagerClass;
    if (!shader_manager_)
        return false;
    result = shader_manager_->Initialize(d3d_->GetDevice(), hwnd);
    if (!result)
        return false;

    texture_manager_ = new TextureManagerClass;
    if (!texture_manager_)
        return false;
    result = texture_manager_->Initialize(5);
    if (!result)
        return false;
    texture_manager_->Load_texture(d3d_->GetDevice(), L"../Engine/data/dirt01d.dds", 0);
    texture_manager_->Load_texture(d3d_->GetDevice(), L"../Engine/data/dirt01n.dds", 1);
    texture_manager_->Load_texture(d3d_->GetDevice(), L"../Engine/data/distance01n.dds", 2);
    texture_manager_->Load_texture(d3d_->GetDevice(), L"../Engine/data/perturb001.dds", 3);
    texture_manager_->Load_texture(d3d_->GetDevice(), L"../Engine/data/cloud001.dds", 4);

    skybox_ = new SkyBoxClass;
    if (!skybox_)
        return false;
    result = skybox_->Initialize(d3d_->GetDevice());
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the skybox object", L"Error", MB_OK);
        return false;
    }

    terrain_ = new TerrainClass;
    if (!terrain_)
        return false;
    result = terrain_->Initialize(d3d_->GetDevice(), "../Engine/data/setup.txt");
    if (!result)
        return false;

    particle_system_ = new ParticleSystemClass;
    if (!particle_system_)
        return false;
    result = particle_system_->Initialize(d3d_->GetDevice(), L"../Engine/data/star.dds");
    if (!result)
        return false;

    ground_model_ = new ModelClass;
    if (!ground_model_)
        return false;
    result = ground_model_->Initialize(d3d_->GetDevice(), "../Engine/data/ground.txt", L"../Engine/data/ground01.dds", L"../Engine/data/ground01_normal.dds", L"../Engine/data/ground01_spec.dds");
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the fround model", L"Error", MB_OK);
        return false;
    }

    wall_model_ = new ModelClass;
    if (!wall_model_)
        return false;
    result = wall_model_->Initialize(d3d_->GetDevice(), "../Engine/data/wall.txt", L"../Engine/data/wall01.dds", L"../Engine/data/wall01_normal.dds", L"../Engine/data/wall01_spec.dds");
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the wall model object", L"error", MB_OK);
        return false;
    }

    bath_model_ = new ModelClass;
    if (!bath_model_)
        return false;
    result = bath_model_->Initialize(d3d_->GetDevice(), "../Engine/data/bath.txt", L"../Engine/data/marble01.dds", L"../Engine/data/marble01_normal.dds", L"../Engine/data/marble01_spec.dds");
    if (!result)
    {
        MessageBox(hwnd, L"could not initialize the bath model", L"Error", MB_OK);
        return false;
    }

    water_model_ = new ModelClass;
    if (!water_model_)
        return false;
    result = water_model_->Initialize(d3d_->GetDevice(), "../Engine/data/water.txt", L"../Engine/data/water01.dds");
    if (!result)
    {
        MessageBox(hwnd, L"could not initialize water model", L"Error", MB_OK);
        return false;
    }

    fire_model_ = new ModelClass;
    if (!fire_model_)
        return false;
    result = fire_model_->Initialize(d3d_->GetDevice(), "../Engine/data/square.txt", L"../Engine/data/fire01.dds", L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");
    if (!result)
    {
        MessageBox(hwnd, L"could not initialize fire model", L"Error", MB_OK);
        return false;
    }

    //create camera object
    camera_ = new CameraClass;
    if (!camera_)
        return false;
    camera_->Set_position(0.0f, 0.0f, -10.0f);
    camera_->Render();
    camera_->Render_static();
    camera_->Get_view_matrix(static_view_matrix);

    text_ = new TextClass;
    if (!text_)
        return false;
    result = text_->Initialize(d3d_->GetDevice(), d3d_->GetDeviceContext(), hwnd, screen_width, screen_height, static_view_matrix);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize text object.", L"Error", MB_OK);
        return false;
    }

    //create model object
    model_ = new ModelClass;
    if (!model_)
        return false;
    //init model
    result = model_->Initialize(d3d_->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/floor.dds", L"../Engine/data/floor_normal.dds", L"../Engine/data/floor_spec.dds");
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"error", MB_OK);
        return false;
    }

    model_list_ = new ModelListClass;
    if (!model_list_)
    {
        return false;
    }
    result = model_list_->Initialize(50);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model list object", L"Error", MB_OK);
        return false;
    }

    shadow_texture_ = new RenderTextureClass;
    if (!shadow_texture_)
        return false;
    result = shadow_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init shadow texture", L"Error", MB_OK);
        return false;
    }

    render_texture_ = new RenderTextureClass;
    if (!render_texture_)
        return false;
    result = render_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init render texture", L"Error", MB_OK);
        return false;
    }

    down_sample_texture_ = new RenderTextureClass;
    if (!down_sample_texture_)
        return false;
    result = down_sample_texture_->Initialize(d3d_->GetDevice(), down_sample_width, down_sample_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init down sample texture", L"Error", MB_OK);
        return false;
    }

    horizontal_blur_texture_ = new RenderTextureClass;
    if (!horizontal_blur_texture_)
        return false;
    result = horizontal_blur_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init horizontal blur texture", L"Error", MB_OK);
        return false;
    }

    vertical_blur_texture_ = new RenderTextureClass;
    if (!vertical_blur_texture_)
        return false;
    result = vertical_blur_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init vertical blur texture", L"Error", MB_OK);
        return false;
    }

    up_sample_texture_ = new RenderTextureClass;
    if (!up_sample_texture_)
        return false;
    result = up_sample_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"could not init up sample render texture", L"Error", MB_OK);
        return false;
    }

    refraction_texture_ = new RenderTextureClass;
    if (!refraction_texture_)
        return false;
    refraction_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height);
    if (!result)
    {
        MessageBox(hwnd, L"could not init refraction render texture", L"Error", MB_OK);
        return false;
    }

    reflection_texture_ = new RenderTextureClass;
    if (!reflection_texture_)        return false;

    result = reflection_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height);
    if (!result)
        return false;

    small_window_ = new OrthoWindowClass;
    if (!small_window_)
        return false;
    result = small_window_->Initialize(d3d_->GetDevice(), down_sample_width, down_sample_height);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the small window object.", L"Error", MB_OK);
        return false;
    }

    full_sceen_window_ = new OrthoWindowClass;
    if (!full_sceen_window_)
        return false;
    result = full_sceen_window_->Initialize(d3d_->GetDevice(), screen_width, screen_height);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the fullscreen window object.", L"Error", MB_OK);
        return false;
    }

    mouse_ = new BitmapClass;
    if (!mouse_)
        return false;
    result = mouse_->Initialize(d3d_->GetDevice(), screen_width, screen_height, L"../Engine/data/mouse.dds", 32, 32);
    if (!result)
    {
        MessageBox(hwnd, L"could not initialize mouse.", L"Error", MB_OK);
        return false;
    }
    begin_check_ = false;

    deferred_buffers_ = new DeferredBuffersClass;
    if (!deferred_buffers_)
        return false;
    result = deferred_buffers_->Initialize(d3d_->GetDevice(), screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
        return false;


    light_ = new LightClass;
    if (!light_)
        return false;
    light_->Set_ambient_color(0.35f, 0.35f, 0.35f, 1.0f);
    light_->Set_diffuse_color(1.0f, 1.0f, 1.0f, 1.0f);
    light_->Set_position(0.0f, 18.0f, 0.5f);
    light_->Set_direction(0.0f, 0.0f, 1.0f);
    light_->Set_specular_color(1.0f, 1.0f, 1.0f, 1.0f);
    light_->Set_specular_power(32.0f);
    light_->Set_look_at(0.0f, 5.0f, 0.0f);
    light_->Generate_ortho_matrix(20.0f, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);

    water_height_ = 2.75f;
    water_translation_ = 0.0f;


    debug_window_ = new DebugWindowClass;
    if (!debug_window_)
        return false;
    result = debug_window_->Initialize(d3d_->GetDevice(), screen_width, screen_height, 100, 100);
    if (!result)
    {
        MessageBox(hwnd, L"Could not init the debug window object", L"Error", MB_OK);
    }

    return true;
}

//kill all graphics objects
void GraphicsClass::Shutdown()
{
    if (shader_manager_)
    {
        shader_manager_->Shutdown();
        delete shader_manager_;
        shader_manager_ = nullptr;
    }

    if (skybox_)
    {
        skybox_->Shutdown();
        delete skybox_;
        skybox_ = nullptr;
    }


    if (text_)
    {
        text_->Shutdown();
        delete text_;
        text_ = nullptr;
    }

    if (mouse_)
    {
        mouse_->Shutdown();
        delete mouse_;
        mouse_ = nullptr;
    }

    if (deferred_buffers_)
    {
        deferred_buffers_->Shutdown();
        delete deferred_buffers_;
        deferred_buffers_ = nullptr;
    }

    if (particle_system_)
    {
        particle_system_->Shutdown();
        delete particle_system_;
        particle_system_ = nullptr;
    }

    if (full_sceen_window_)
    {
        full_sceen_window_->Shutdown();
        delete full_sceen_window_;
        full_sceen_window_ = nullptr;
    }

    if (small_window_)
    {
        small_window_->Shutdown();
        delete small_window_;
        small_window_ = nullptr;
    }

    if (up_sample_texture_)
    {
        up_sample_texture_->Shutdown();
        delete up_sample_texture_;
        up_sample_texture_ = nullptr;
    }

    if (vertical_blur_texture_)
    {
        vertical_blur_texture_->Shutdown();
        delete vertical_blur_texture_;
        vertical_blur_texture_ = nullptr;
    }

    if (horizontal_blur_texture_)
    {
        horizontal_blur_texture_->Shutdown();
        delete horizontal_blur_texture_;
        horizontal_blur_texture_ = nullptr;
    }
    if (down_sample_texture_)
    {
        down_sample_texture_->Shutdown();
        delete down_sample_texture_;
        down_sample_texture_ = nullptr;
    }

    if (render_texture_)
    {
        render_texture_->Shutdown();
        delete render_texture_;
        render_texture_ = nullptr;
    }

    if (refraction_texture_)
    {
        refraction_texture_->Shutdown();
        delete refraction_texture_;
        refraction_texture_ = nullptr;
    }

    if (debug_window_)
    {
        debug_window_->Shutdown();
        delete debug_window_;
        debug_window_ = nullptr;
    }

    if (reflection_texture_)
    {
        reflection_texture_->Shutdown();
        delete reflection_texture_;
        reflection_texture_ = nullptr;
    }

    if (fire_model_)
    {
        fire_model_->Shutdown();
        delete fire_model_;
        fire_model_ = nullptr;
    }

    if (floor_model_)
    {
        floor_model_->Shutdown();
        delete floor_model_;
        floor_model_ = nullptr;
    }

    if (water_model_)
    {
        water_model_->Shutdown();
        delete water_model_;
        water_model_ = nullptr;
    }

    if (bath_model_)
    {
        bath_model_->Shutdown();
        delete bath_model_;
        bath_model_ = nullptr;
    }

    if (wall_model_)
    {
        wall_model_->Shutdown();
        delete wall_model_;
        wall_model_ = nullptr;
    }

    if (ground_model_)
    {
        ground_model_->Shutdown();
        delete ground_model_;
        ground_model_ = nullptr;
    }

    if (model_list_)
    {
        model_list_->Shutdown();
        delete model_list_;
        model_list_ = nullptr;
    }

    if (light_)
    {
        delete light_;
        light_ = nullptr;
    }

    if (model_)
    {
        model_->Shutdown();
        delete model_;
        model_ = nullptr;
    }

    if (camera_)
    {
        delete camera_;
        camera_ = nullptr;
    }

    if (d3d_)
    {
        d3d_->Shutdown();
        delete d3d_;
        d3d_ = nullptr;
    }
}

bool GraphicsClass::Frame(float frame_time, float rotation_x, float rotation_y, float x_pos, float z_pos, float mouse_x, float mouse_y)
{
    static float light_angle = 270.0f;
    float radians;
    static float light_pos_x = 9.0f;
    static float frame_count = 0;
    static float frame_acum = 0;

    bool found_height;
    float height;

    mouse_x = -mouse_x / screen_width_;
    mouse_y = -mouse_y / screen_height_;
    if ((abs(mouse_x) + abs(mouse_y)) > abs(mouse_delta_x_) + abs(mouse_delta_y_))
    {
        mouse_delta_x_ = mouse_x;
        mouse_delta_y_ = mouse_y;
    }
    else
    {
        mouse_delta_x_ -= mouse_delta_x_ * 0.168033f;
        mouse_delta_y_ -= mouse_delta_y_ * 0.168033f;
    }


    frame_acum += frame_time;
    frame_count++;
    if (frame_acum >= 1000.0f)
    {
        text_->Set_fps(frame_count, d3d_->GetDeviceContext());
        frame_count = 0;
        frame_acum = 0;
    }

    light_pos_x -= 0.003f * frame_time / 10;
    light_angle -= 0.03f * frame_time / 10;
    if (light_angle <= 90.0f)
    {
        light_angle = 270.0f;
        light_pos_x = 9.0f;
    }

    radians = light_angle * 0.0174532925f;
    light_->Set_direction(sinf(radians), cosf(radians), 0.0f);
    light_->Set_position(light_pos_x, 15.0f, -0.1f);
    light_->Set_look_at(-light_pos_x, 0.0f, 0.0f);

    water_translation_ += 0.001f;
    if (water_translation_ > 1.0)
        water_translation_ -= 1.0f;

    rotation_x_ = rotation_x;
    rotation_y_ = rotation_y;

    found_height = terrain_->Get_height_at_position(x_pos, z_pos, height);
    if (found_height)
        camera_->Set_position(x_pos, height + 5.0f, z_pos);
    else
        camera_->Set_position(x_pos, 100.0f, z_pos);

    camera_->Set_rotation(rotation_x, rotation_y, 0.0f);

    frame_time_ += frame_time / 1000.0f;
    if (frame_time_ >= 1000.0f)
        frame_time_ = 0.0f;

    terrain_->Frame();

    particle_system_->Frame(frame_time, d3d_->GetDeviceContext());
    return true;
}

bool GraphicsClass::Render()
{
    bool result;


    result = Render_refraction_to_texture();
    if (!result)
        return false;

    result = Render_reflection_to_texture();
    if (!result)
        return false;

    //Render_scene();
    result = Render_light_scene_to_texture();
    if (!result)
        return false;

    result = Render_scene_to_texture();
    if (!result)
        return false;
    //ugly
    /*
    result = Down_sample_texture();
    if(!result)
    return false;
    */

    result = Render_horizontal_bloor_to_texture();
    if (!result)
        return false;

    result = Render_vertical_bloor_to_texture();
    if (!result)
        return false;
    //ugly
    /*
    result = Up_sample_texture();
    if (!result)
    return false;
    */
    result = Render2d_texture_scene();
    if (!result)
        return false;

    return true;
}

bool GraphicsClass::Handle_input(bool lmb, int mouse_x, int mouse_y)
{
    bool result;
    mouse_x_ = mouse_x;
    mouse_y_ = mouse_y;

    if (lmb)
    {
        if (!begin_check_)
        {
            begin_check_ = true;
            Test_intersection();
        }
    }
    else
        begin_check_ = false;
    return true;
}

void GraphicsClass::Test_intersection()
{
    float point_x, point_y;
    D3DXMATRIX projection, view, inverse_view, world, translate, inverse_world;
    D3DXVECTOR3 direction, origin, ray_origin, ray_direction;
    bool intersect, result;

    point_x = ((2.0f * (float)mouse_x_) / (float)screen_width_) - 1.0f;
    point_y = (((2.0f * (float)mouse_y_) / (float)screen_height_) - 1.0f) * -1.0f;

    d3d_->GetProjectionMatrix(projection);
    point_x = point_x / projection._11;
    point_y = point_y / projection._22;

    camera_->Get_view_matrix(view);
    D3DXMatrixInverse(&inverse_view, nullptr, &view);

    direction.x = (point_x * inverse_view._11) + (point_y * inverse_view._21) + inverse_view._31;
    direction.y = (point_x * inverse_view._12) + (point_y * inverse_view._22) + inverse_view._32;
    direction.z = (point_x * inverse_view._13) + (point_y * inverse_view._23) + inverse_view._33;

    origin = camera_->Get_position();

    d3d_->GetWorldMatrix(world);
    D3DXMatrixTranslation(&translate, 0.0f, 5.0f, 0.0f);
    D3DXMatrixMultiply(&world, &world, &translate);

    D3DXMatrixInverse(&inverse_world, nullptr, &world);

    D3DXVec3TransformCoord(&ray_origin, &origin, &inverse_world);
    D3DXVec3TransformNormal(&ray_direction, &direction, &inverse_world);

    D3DXVec3Normalize(&ray_direction, &ray_direction);

    intersect = Ray_sphere_intersect(ray_origin, ray_direction, 1.0f);
    fire_on_ = intersect;
}

bool GraphicsClass::Ray_sphere_intersect(D3DXVECTOR3 ray_origin, D3DXVECTOR3 ray_direction, float radius)
{
    float a, b, c, discriminant;

    a = (ray_direction.x * ray_direction.x) + (ray_direction.y * ray_direction.y) + (ray_direction.z * ray_direction.z);
    b = (ray_direction.x * ray_origin.x) + (ray_direction.y * ray_origin.y) + (ray_direction.z * ray_origin.z) * 2.0f;
    c = (ray_origin.x * ray_origin.x) + (ray_origin.y * ray_origin.y) + (ray_origin.z * ray_origin.z) - (radius * radius);

    discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0.0f)
        return false;
    return true;
}

bool GraphicsClass::Render_scene()
{
    D3DXMATRIX world_matrix, view_matrix, projection_matrix, reflection_matrix, translate_matrix;
    bool result;

    D3DXVECTOR3 camera_position, model_position;
    double angle;
    float rotation;

    D3DXVECTOR3 scroll_speed, scale;
    D3DXVECTOR2 distortion1, distortion2, distortion3;
    float distortion_scale, distortion_bias;

    float frame_time = frame_time_;

    //three scrolling speeds for three different noise txtrs
    scroll_speed = D3DXVECTOR3(1.3f, 2.1f, 2.3f);
    //three scales for three different noise octave txtrs
    scale = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

    //three different x and y distortion factors for noise txtrs
    distortion1 = D3DXVECTOR2(0.1f, 0.2f);
    distortion2 = D3DXVECTOR2(0.1f, 0.3f);
    distortion3 = D3DXVECTOR2(0.1f, 0.1f);

    distortion_scale = 0.8f;
    distortion_bias = 0.5f;

    d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);

    camera_->Render();

    d3d_->GetWorldMatrix(world_matrix);
    camera_->Get_view_matrix(view_matrix);
    d3d_->GetProjectionMatrix(projection_matrix);

    //ground
    D3DXMatrixTranslation(&world_matrix, 0.0f, 1.0f, 0.0f);
    ground_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), ground_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, ground_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //wall
    D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
    wall_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, wall_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //fire
    if (fire_on_)
    {
        d3d_->Turn_zbuffer_off();
        d3d_->TurnOnAlphaBlending();
        camera_position = camera_->Get_position();
        model_position.x = 0.0f;
        model_position.y = water_height_;
        model_position.z = 7.0f;
        angle = atan2(model_position.x - camera_position.x, model_position.z - camera_position.z) * (180.0f / D3DX_PI);
        rotation = static_cast<float>(angle) * 0.0174532925f;

        D3DXMatrixRotationY(&world_matrix, rotation);
        D3DXMatrixTranslation(&translate_matrix, model_position.x, model_position.y, model_position.z);
        D3DXMatrixMultiply(&world_matrix, &world_matrix, &translate_matrix);

        fire_model_->Render(d3d_->GetDeviceContext());
        result = shader_manager_->Render_fire_shader(d3d_->GetDeviceContext(), fire_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, fire_model_->Get_textures()[0], fire_model_->Get_textures()[1], fire_model_->Get_textures()[2], frame_time, scroll_speed, scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
        if (!result)
            return false;
        d3d_->TurnOffAlphaBlending();
        d3d_->Turn_zbuffer_on();
    }
    //bath
    D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
    bath_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, bath_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    reflection_matrix = camera_->Get_reflection_view_matrix();

    //polandball
    D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
    model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //water
    D3DXMatrixTranslation(&world_matrix, 0.0f, water_height_, 0.0f);
    water_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_water_shader(d3d_->GetDeviceContext(), water_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, reflection_matrix, reflection_texture_->Get_shader_resource_view(), refraction_texture_->Get_shader_resource_view(), water_model_->Get_texture(), water_translation_, 0.05f);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);
    d3d_->End_scene();
    return true;
}

bool GraphicsClass::Render_light_scene_to_texture()
{
    D3DXMATRIX world_matrix, light_view_matrix, light_ortho_matrix;
    bool result;


    shadow_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    shadow_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    //d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);

    light_->Generate_view_matrix();
    light_->Get_view_matrix(light_view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    light_->Get_ortho_matrix(light_ortho_matrix);

    //wall
    D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
    wall_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_depth_shader(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, light_view_matrix, light_ortho_matrix);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //bath
    D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
    bath_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_depth_shader(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, light_view_matrix, light_ortho_matrix);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //polandball
    D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
    model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_depth_shader(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, light_view_matrix, light_ortho_matrix);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //d3d_->End_scene();
    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Up_sample_texture()
{
    D3DXMATRIX world_matrix, view_matrix, ortho_matrix;
    bool result;

    up_sample_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    up_sample_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    camera_->Render();

    camera_->Get_view_matrix(view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    up_sample_texture_->Get_ortho_matrix(ortho_matrix);
    d3d_->Turn_zbuffer_off();

    D3DXMatrixRotationYawPitchRoll(&world_matrix, rotation_y_* 0.0174532925f, rotation_x_ * 0.0174532925f, 0.0f);
    full_sceen_window_->Render(d3d_->GetDeviceContext());
    //result = shader_manager_->Render_texture_shader(d3d_->GetDeviceContext(), full_sceen_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, vertical_blur_texture_->Get_shader_resource_view());
    // if (!result)
    //    return false;

    d3d_->Turn_zbuffer_on();
    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Down_sample_texture()
{
    D3DXMATRIX world_matrix, view_matrix, ortho_matrix;
    bool result;

    down_sample_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    down_sample_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    camera_->Render();
    camera_->Get_view_matrix(view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    down_sample_texture_->Get_ortho_matrix(ortho_matrix);
    //D3DXMatrixOrthoOffCenterLH(&ortho_matrix, -down_sample_texture_->Get_texture_width() / 2, down_sample_texture_->Get_texture_width()/2, -down_sample_texture_->Get_texture_height() / 2, down_sample_texture_->Get_texture_height() / 2, SCREEN_NEAR, SCREEN_DEPTH);

    d3d_->Turn_zbuffer_off();
    D3DXMatrixRotationYawPitchRoll(&world_matrix, rotation_y_* 0.0174532925f, rotation_x_ * 0.0174532925f, 0.0f);
    small_window_->Render(d3d_->GetDeviceContext());
//    result = shader_manager_->Render_texture_shader(d3d_->GetDeviceContext(), small_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, render_texture_->Get_shader_resource_view());
//    if (!result)
//        return false;

    d3d_->Turn_zbuffer_on();
    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Render_refraction_to_texture()
{
    D3DXVECTOR4 clip_plane;
    D3DXMATRIX world_matrix, view_matrix, projection_matrix;
    bool result;

    clip_plane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, water_height_ + 0.1f);

    refraction_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    refraction_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

    camera_->Render();

    d3d_->GetWorldMatrix(world_matrix);
    camera_->Get_view_matrix(view_matrix);
    d3d_->GetProjectionMatrix(projection_matrix);

    D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);

    bath_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_refraction_shader(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, bath_model_->Get_texture(), light_->Get_position(), light_->Get_ambient_color(), light_->Get_diffuse_color(), clip_plane);
    if (!result)
        return false;

    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}


bool GraphicsClass::Render_reflection_to_texture()
{
    D3DXMATRIX world_matrix, reflection_view_matrix, projection_matrix;
    bool result;
    D3DXVECTOR3 scroll_speed, scale;
    D3DXVECTOR2 distortion1, distortion2, distortion3;
    float distortion_scale, distortion_bias;

    float frame_time = frame_time_;

    //three scrolling speeds for three different noise txtrs
    scroll_speed = D3DXVECTOR3(1.3f, 2.1f, 2.3f);
    //three scales for three different noise octave txtrs
    scale = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

    //three different x and y distortion factors for noise txtrs
    distortion1 = D3DXVECTOR2(0.1f, 0.2f);
    distortion2 = D3DXVECTOR2(0.1f, 0.3f);
    distortion3 = D3DXVECTOR2(0.1f, 0.1f);

    distortion_scale = 0.8f;
    distortion_bias = 0.5f;

    reflection_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    reflection_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

    camera_->Render_reflection(water_height_);
    reflection_view_matrix = camera_->Get_reflection_view_matrix();

    d3d_->GetWorldMatrix(world_matrix);
    d3d_->GetProjectionMatrix(projection_matrix);

    D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
    bath_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, bath_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;

    d3d_->GetWorldMatrix(world_matrix);
    D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
    wall_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, wall_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;

    d3d_->GetWorldMatrix(world_matrix);

    D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
    model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    D3DXMatrixTranslation(&world_matrix, 0.0f, 0.0f, 7.0f);
    d3d_->TurnOnAlphaBlending();
    fire_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_fire_shader(d3d_->GetDeviceContext(), fire_model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, fire_model_->Get_textures()[0], fire_model_->Get_textures()[1], fire_model_->Get_textures()[2], frame_time, scroll_speed, scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //particles
    particle_system_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_particle_shader(d3d_->GetDeviceContext(), particle_system_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, particle_system_->Get_texture());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);
    d3d_->TurnOffAlphaBlending();

    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();

    return true;
}


bool GraphicsClass::Render_scene_to_texture()
{
    D3DXMATRIX world_matrix, view_matrix, static_view_matrix, projection_matrix, reflection_matrix, translate_matrix, ortho_matrix;
    D3DXMATRIX light_view_matrix, light_ortho_matrix;
    bool result;

    D3DXVECTOR3 camera_position, model_position;
    double angle;
    float rotation;
    D3DXVECTOR3 light_dir;
    float frame_time = frame_time_;

    D3DXVECTOR3 scroll_speed, scale;
    D3DXVECTOR2 distortion1, distortion2, distortion3;
    float distortion_scale, distortion_bias;

    //three scrolling speeds for three different noise txtrs
    scroll_speed = D3DXVECTOR3(1.3f, 2.1f, 2.3f);
    //three scales for three different noise octave txtrs
    scale = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

    //three different x and y distortion factors for noise txtrs
    distortion1 = D3DXVECTOR2(0.1f, 0.2f);
    distortion2 = D3DXVECTOR2(0.1f, 0.3f);
    distortion3 = D3DXVECTOR2(0.1f, 0.1f);

    distortion_scale = 0.8f;
    distortion_bias = 0.5f;
    deferred_buffers_->Set_render_targets(d3d_->GetDeviceContext());
    deferred_buffers_->Clear_render_targets(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    //d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);



    light_->Generate_view_matrix();
    camera_->Get_view_matrix(view_matrix);
    camera_->Get_static_view_matrix(static_view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    d3d_->GetProjectionMatrix(projection_matrix);
    d3d_->GetOrthoMatrix(ortho_matrix);
    light_->Get_view_matrix(light_view_matrix);
    light_->Get_ortho_matrix(light_ortho_matrix);

    camera_position = camera_->Get_position();

    camera_->Render();
    d3d_->Turn_culling_off();
    d3d_->Turn_zbuffer_off();
    D3DXMatrixTranslation(&world_matrix, camera_position.x, camera_position.y, camera_position.z);
    skybox_->Render(d3d_->GetDeviceContext());

    //skybox
    light_dir = light_->Get_direction();

    result = shader_manager_->Render_skybox_shader(d3d_->GetDeviceContext(), skybox_->Get_index_count(), world_matrix, view_matrix, projection_matrix, texture_manager_->Get_texture(3), texture_manager_->Get_texture(4), frame_time_, skybox_->Get_scale(), -light_dir.y, skybox_->Get_apex_color(), skybox_->Get_center_color());
    if (!result)
        return false;
    d3d_->Turn_zbuffer_on();
    d3d_->Turn_culling_on();


    //ground
    d3d_->GetWorldMatrix(world_matrix);
    frustum_->ConstructFrustrum(SCREEN_DEPTH, projection_matrix, view_matrix);
    for (int i = 0; i < terrain_->Get_cell_count(); i++)
    {
        result = terrain_->Render_cell(d3d_->GetDeviceContext(), i, frustum_);
        if (result)
        {
            result = shader_manager_->Render_terrain_shader(d3d_->GetDeviceContext(), terrain_->Get_cell_index_count(i), world_matrix, view_matrix, projection_matrix, light_view_matrix, light_ortho_matrix, texture_manager_->Get_texture(0), shadow_texture_->Get_shader_resource_view(), texture_manager_->Get_texture(1), texture_manager_->Get_texture(2), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color());
            if (!result)
                return false;
        }
    }
    d3d_->GetWorldMatrix(world_matrix);

    //wall
    D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
    wall_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, wall_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    if (!fire_on_)
    {
        //fire
        d3d_->Turn_zbuffer_off();
        d3d_->TurnOnAlphaBlending();
        camera_position = camera_->Get_position();
        model_position.x = 0.0f;
        model_position.y = 0.0f;
        model_position.z = 7.0f;
        angle = atan2(model_position.x - camera_position.x, model_position.z - camera_position.z) * (180.0f / D3DX_PI);
        rotation = static_cast<float>(angle) * 0.0174532925f;

        D3DXMatrixRotationY(&world_matrix, rotation);
        D3DXMatrixTranslation(&translate_matrix, model_position.x, model_position.y, model_position.z);
        D3DXMatrixMultiply(&world_matrix, &world_matrix, &translate_matrix);

        fire_model_->Render(d3d_->GetDeviceContext());
        result = shader_manager_->Render_fire_shader(d3d_->GetDeviceContext(), fire_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, fire_model_->Get_textures()[0], fire_model_->Get_textures()[1], fire_model_->Get_textures()[2], frame_time, scroll_speed, scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
        if (!result)
            return false;
        d3d_->TurnOffAlphaBlending();
        d3d_->Turn_zbuffer_on();
    }
    //bath
    D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
    bath_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, bath_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    reflection_matrix = camera_->Get_reflection_view_matrix();

    //polandball
    D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
    model_->Render(d3d_->GetDeviceContext());
    //result = deferred_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_textures()[0], model_->Get_textures()[1]);
    result = shader_manager_->Render_normal_shader(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //water
    D3DXMatrixTranslation(&world_matrix, 0.0f, water_height_, 0.0f);
    water_model_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_water_shader(d3d_->GetDeviceContext(), water_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, reflection_matrix, reflection_texture_->Get_shader_resource_view(), refraction_texture_->Get_shader_resource_view(), water_model_->Get_texture(), water_translation_, 0.05f);
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    //particles
    d3d_->TurnOnAlphaBlending();
    camera_position = camera_->Get_position();
    model_position.x = 0.0f;
    model_position.y = -1.0f;
    model_position.z = 0.0f;
    angle = atan2(model_position.x - camera_position.x, model_position.z - camera_position.z) * (180.0f / D3DX_PI);
    rotation = static_cast<float>(angle) * 0.0174532925f;

    D3DXMatrixRotationY(&world_matrix, rotation);
    D3DXMatrixTranslation(&translate_matrix, model_position.x, model_position.y, model_position.z);
    D3DXMatrixMultiply(&world_matrix, &world_matrix, &translate_matrix);
    particle_system_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_particle_shader(d3d_->GetDeviceContext(), particle_system_->Get_index_count(), world_matrix, view_matrix, projection_matrix, particle_system_->Get_texture());
    if (!result)
        return false;
    d3d_->GetWorldMatrix(world_matrix);

    result = text_->Render(d3d_->GetDeviceContext(), world_matrix, ortho_matrix);
    if (!result)
        return false;

    //mouse
    result = mouse_->Render(d3d_->GetDeviceContext(), screen_width_/2, screen_height_/2);
    if (!result)
        return false;

   // result = shader_manager_->Render_texture_shader(d3d_->GetDeviceContext(), mouse_->Get_index_count(), world_matrix, static_view_matrix, ortho_matrix, mouse_->Get_texture());
    if (!result)
        return false;

    d3d_->TurnOffAlphaBlending();
    d3d_->GetWorldMatrix(world_matrix);

    //d3d_->End_scene();
    d3d_->Set_back_buffer_render_target();

    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Render_horizontal_bloor_to_texture()
{
    D3DXMATRIX world_matrix, view_matrix, ortho_matrix;
    float screen_size_x;
    bool result;

    screen_size_x = (float)horizontal_blur_texture_->Get_texture_width();
    horizontal_blur_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    horizontal_blur_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

    camera_->Render();

    camera_->Get_view_matrix(view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    horizontal_blur_texture_->Get_ortho_matrix(ortho_matrix);
    d3d_->Turn_zbuffer_off();

    D3DXMatrixRotationYawPitchRoll(&world_matrix, rotation_y_* 0.0174532925f, rotation_x_ * 0.0174532925f, 0.0f);
    full_sceen_window_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_horizontal_blur_shader(d3d_->GetDeviceContext(), small_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, render_texture_->Get_shader_resource_view(), screen_size_x);
    if (!result)
        return false;

    d3d_->Turn_zbuffer_on();
    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Render_vertical_bloor_to_texture()
{
    D3DXMATRIX world_matrix, view_matrix, ortho_matrix;
    float screen_size_y;
    bool result;

    screen_size_y = (float)vertical_blur_texture_->Get_texture_height();

    vertical_blur_texture_->Set_render_target(d3d_->GetDeviceContext());
    d3d_->Reset_viewport();
    vertical_blur_texture_->Clear_render_target(d3d_->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    camera_->Render();

    camera_->Get_view_matrix(view_matrix);
    d3d_->GetWorldMatrix(world_matrix);

    vertical_blur_texture_->Get_ortho_matrix(ortho_matrix);

    d3d_->Turn_zbuffer_off();
    D3DXMatrixRotationYawPitchRoll(&world_matrix, rotation_y_* 0.0174532925f, rotation_x_ * 0.0174532925f, 0.0f);
    full_sceen_window_->Render(d3d_->GetDeviceContext());
    result = shader_manager_->Render_vertical_blur_shader(d3d_->GetDeviceContext(), small_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, horizontal_blur_texture_->Get_shader_resource_view(), screen_size_y);
    if (!result)
        return result;

    d3d_->Turn_zbuffer_on();
    d3d_->Set_back_buffer_render_target();
    d3d_->Reset_viewport();
    return true;
}

bool GraphicsClass::Render2d_texture_scene()
{
    D3DXMATRIX world_matrix, view_matrix, ortho_matrix;
    bool result;

    d3d_->Begin_scene(1.0f, 0.0f, 0.0f, 0.0f);
    camera_->Render();
    camera_->Get_static_view_matrix(view_matrix);
    d3d_->GetWorldMatrix(world_matrix);
    d3d_->GetOrthoMatrix(ortho_matrix);
    d3d_->Turn_zbuffer_off();

    full_sceen_window_->Render(d3d_->GetDeviceContext());
    //result = light_shader_->Render(d3d_->GetDeviceContext(), full_sceen_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, deferred_buffers_->Get_shader_resource_view(0), deferred_buffers_->Get_shader_resource_view(1), light_->Get_direction());
    result = shader_manager_->Render_texture_shader(d3d_->GetDeviceContext(), full_sceen_window_->Get_index_count(), world_matrix, view_matrix, ortho_matrix, deferred_buffers_->Get_shader_resource_view(0), mouse_delta_x_, mouse_delta_y_);
    if (!result)
        return false;

    d3d_->Turn_zbuffer_on();
    d3d_->End_scene();
    return true;
}