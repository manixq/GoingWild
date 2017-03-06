#include "graphics_class.h"

GraphicsClass::GraphicsClass()
{
 d3d_ = nullptr;
 camera_ = nullptr;
 model_ = nullptr;
 normal_shader_ = nullptr;
 light_ = nullptr;
 model_list_ = nullptr;
 frustum_ = nullptr;
 floor_model_ = nullptr;
 debug_window_ = nullptr;

 ground_model_ = nullptr;
 wall_model_ = nullptr;
 bath_model_ = nullptr;
 water_model_ = nullptr;
 fire_model_ = nullptr;

 reflection_texture_ = nullptr;
 refraction_texture_ = nullptr;

 reflection_shader_ = nullptr;
 refraction_shader_ = nullptr;
 water_shader_ = nullptr;
 fire_shader_ = nullptr;
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

 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
  return false;
 }
 ground_model_ = new ModelClass;
 if (!ground_model_)
  return false;

 result = ground_model_->Initialize(d3d_->GetDevice(), "../Engine/data/ground.txt", L"../Engine/data/ground01.dds", L"../Engine/data/ground01_normal.dds", L"../Engine/data/ground01_spec.dds");
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the fround model", L"Error", MB_OK);
  return false;
 }

 wall_model_ = new ModelClass;
 if (!wall_model_)
  return false;

 result = wall_model_->Initialize(d3d_->GetDevice(), "../Engine/data/wall.txt", L"../Engine/data/wall01.dds", L"../Engine/data/wall01_normal.dds", L"../Engine/data/wall01_spec.dds");
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the wall model object", L"error", MB_OK);
  return false;
 }

 bath_model_ = new ModelClass;
 if (!bath_model_)
  return false;

 result = bath_model_->Initialize(d3d_->GetDevice(), "../Engine/data/bath.txt", L"../Engine/data/marble01.dds", L"../Engine/data/marble01_normal.dds", L"../Engine/data/marble01_spec.dds");
 if(!result)
 {
  MessageBox(hwnd, L"could not initialize the bath model", L"Error", MB_OK);
  return false;
 }

 water_model_ = new ModelClass;
 if (!water_model_)
  return false;

 result = water_model_->Initialize(d3d_->GetDevice(), "../Engine/data/water.txt", L"../Engine/data/water01.dds");
 if(!result)
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
 
 //create model object
 model_ = new ModelClass;
 if (!model_)
  return false;

 //init model
 result = model_->Initialize(d3d_->GetDevice(), "../Engine/data/sphere.txt" ,L"../Engine/data/floor.dds", L"../Engine/data/floor_normal.dds", L"../Engine/data/floor_spec.dds");
 if (!result)
 {
  MessageBox(hwnd, L"Could not initialize the model object.", L"error", MB_OK);
  return false;
 }

 fire_shader_ = new FireShaderClass;
 if (!fire_shader_)
  return false;

 result = fire_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize fire shader obj", L"Error", MB_OK);
  return false;
 }

 //create color shader object
 normal_shader_ = new NormalShaderClass;
 if (!normal_shader_)
  return false;

 result = normal_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
  return false;
 }

 light_ = new LightClass;
 if (!light_)
  return false;

 model_list_ = new ModelListClass;
 if(!model_list_)
 {
  return false;
 }

 result = model_list_->Initialize(50);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the model list object", L"Error", MB_OK);
  return false;
 }

 refraction_texture_ = new RenderTextureClass;
 if (!refraction_texture_)
  return false;

 refraction_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height);
 if(!result)
 {
  MessageBox(hwnd, L"could not init refraction render texture", L"Error", MB_OK);
  return false;
 }

 reflection_texture_ = new RenderTextureClass;
 if (!reflection_texture_)
  return false;
 
 result = reflection_texture_->Initialize(d3d_->GetDevice(), screen_width, screen_height);
 if (!result)
  return false;

 refraction_shader_ = new RefractionShaderClass;
 if (!refraction_shader_)
  return false;

 refraction_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if (!result)
 {
  MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
  return false;
 }

 reflection_shader_ = new ReflectionShaderClass;
 if (!reflection_shader_)
  return false;

 result = reflection_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
  return false;
 }

 water_shader_ = new WaterShaderClass;
 if (!water_shader_)
  return false;

 result =  water_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
  return false;
 }

 water_height_ = 2.75f;
 water_translation_ = 0.0f;

 frustum_ = new FrustumClass;
 if (!frustum_)
  return false;

 light_->Set_ambient_color(0.35f, 0.35f, 0.35f, 1.0f);
 light_->Set_diffuse_color(1.0f, 1.0f, 1.0f, 1.0f);
 light_->Set_direction(0.0f, -1.0f, 0.5f);
 light_->Set_specular_color(1.0f, 1.0f, 1.0f, 1.0f);
 light_->Set_specular_power(32.0f);

 debug_window_ = new DebugWindowClass;
 if (!debug_window_)
  return false;

 result = debug_window_->Initialize(d3d_->GetDevice(), screen_width, screen_height, 100, 100);
 if(!result)
 {
  MessageBox(hwnd, L"Could not init the debug window object", L"Error", MB_OK);
 }

 return true;
}

//kill all graphics objects
void GraphicsClass::Shutdown()
{
 if(fire_shader_)
 {
  fire_shader_->Shutdown();
  fire_shader_ = nullptr;
 }

 if(refraction_shader_)
 {
  refraction_shader_->Shutdown();
  refraction_shader_ = nullptr;
 }

 if(water_shader_)
 {
  water_shader_->Shutdown();
  water_shader_ = nullptr;
 }

 if(refraction_texture_)
 {
  refraction_texture_->Shutdown();
  refraction_texture_ = nullptr;
 }

 if(reflection_shader_)
 {
  reflection_shader_->Shutdown();
  reflection_shader_ = nullptr;
 }

 if(debug_window_)
 {
  debug_window_->Shutdown();
  delete debug_window_;
  debug_window_ = nullptr;
 }

 if(reflection_texture_)
 {
  reflection_texture_->Shutdown();
  delete reflection_texture_;
  reflection_texture_ = nullptr;
 }

 if(fire_model_)
 {
  fire_model_->Shutdown();
  fire_model_ = nullptr;
 }

 if(floor_model_)
 {
  floor_model_->Shutdown();
  delete floor_model_;
  floor_model_ = nullptr;
 }

 if(water_model_)
 {
  water_model_->Shutdown();
  water_model_ = nullptr;
 }

 if(bath_model_)
 {
  bath_model_->Shutdown();
  bath_model_ = nullptr;
 }

 if(wall_model_)
 {
  wall_model_->Shutdown();
  wall_model_ = nullptr;
 }

 if(ground_model_)
 {
  ground_model_->Shutdown();
  ground_model_ = nullptr;
 }

 if(frustum_)
 {
  delete frustum_;
  frustum_ = nullptr;
 }

 if(model_list_)
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

 if(normal_shader_)
 {
  normal_shader_->Shutdown();
  delete normal_shader_;
  normal_shader_ = nullptr;
 }

 if(model_)
 {
  model_->Shutdown();
  delete model_;
  model_ = nullptr;
 }

 if(camera_)
 {
  delete camera_;
  camera_ = nullptr;
 }

 if(d3d_)
 {
  d3d_->Shutdown();
  delete d3d_;
  d3d_ = nullptr;
 }
}

bool GraphicsClass::Frame(float rotation_x, float rotation_y, float x_pos, float z_pos)
{ 
 water_translation_ += 0.001f;
 if (water_translation_ > 1.0)
  water_translation_ -= 1.0f;
 //camera_->Set_position(-10.0f, 6.0f, -10.0f);
 //camera_->Set_rotation(0.0f, 45.0f, 0.0f);
 camera_->Set_position(x_pos, 6.0f, z_pos);
 camera_->Set_rotation(rotation_x, rotation_y, 0.0f);
 return true;
}

bool GraphicsClass::Render()
{
 D3DXMATRIX view_matrix, projection_matrix, world_matrix, ortho_matrix, reflection_matrix;
 int model_count, render_count, index;
 float position_x, position_y, position_z, radius;
 D3DXVECTOR4 color;
 bool render_model, result;
 D3DXVECTOR4 clip_plane;

 result = Render_refraction_to_texture();
 if (!result)
  return false;

 result = Render_reflection_to_texture();
 if (!result)
  return false;

 result = Render_scene();
 if (!result)
  return false;
 /*clip_plane = D3DXVECTOR4(1.0f, -1.0f, 0.0f, 0.0f);
 d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);
 camera_->Render();
 d3d_->GetWorldMatrix(world_matrix);
 camera_->Get_view_matrix(view_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);
 d3d_->GetOrthoMatrix(ortho_matrix);
 frustum_->ConstructFrustrum(SCREEN_DEPTH, projection_matrix, view_matrix);
 model_count = model_list_->Get_model_count();
 render_count = 0;
 reflection_matrix = camera_->Get_reflection_view_matrix();

 result = debug_window_->Render(d3d_->GetDeviceContext(), 50, 50);
 if (!result)
  return false;

 for (index = 0; index < model_count; index++)
 {
  model_list_->Get_data(index, position_x, position_y, position_z, color);
  radius = 1.0f;
  render_model = frustum_->Check_sphere(position_x, position_y, position_z, radius);
  if(render_model)
  {
   D3DXMatrixTranslation(&world_matrix, position_x, position_y, position_z);
   model_->Render(d3d_->GetDeviceContext());
   result = normal_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_texture(), *floor_model_->Get_texture(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power(), clip_plane, reflection_matrix);
   if (!result)
    return false;
   d3d_->GetWorldMatrix(world_matrix);
   render_count++;
  }
 }
 d3d_->Turn_zbuffer_on();
 //present on screen
 d3d_->End_scene();*/
 return true;
}

bool GraphicsClass::Render_to_texture()
{
 D3DXMATRIX world_matrix, reflection_view_matrix, projection_matrix;
 static float rotation = 0.0f;
 bool result;

 reflection_texture_->Set_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view());

 reflection_texture_->Clear_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view(), 0.0f, 0.0f, 0.0f, 1.0f);

 camera_->Render_reflection(-15.5f);
 reflection_view_matrix = camera_->Get_reflection_view_matrix();

 d3d_->GetWorldMatrix(world_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);

 rotation += (float)D3DX_PI * 0.005f;
 if (rotation > 360.0f)
 {
  rotation -= 360.0f;
 }
 D3DXMatrixRotationY(&world_matrix, rotation);

 model_->Render(d3d_->GetDeviceContext());

 normal_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 
 d3d_->Set_back_buffer_render_target();
 return true;
}

bool GraphicsClass::Render_refraction_to_texture()
{
 D3DXVECTOR4 clip_plane;
 D3DXMATRIX world_matrix, view_matrix, projection_matrix;
 bool result;

 clip_plane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, water_height_+ 0.1f);

 refraction_texture_->Set_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view());

 refraction_texture_->Clear_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view(), 0.0f, 0.0f, 0.0f, 1.0f);

 camera_->Render();


 d3d_->GetWorldMatrix(world_matrix);
 camera_->Get_view_matrix(view_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);

 bath_model_->Render(d3d_->GetDeviceContext());
 result = refraction_shader_->Render(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, bath_model_->Get_texture(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), clip_plane);
 if (!result)
  return false;

 d3d_->Set_back_buffer_render_target();
 return true;
}


bool GraphicsClass::Render_reflection_to_texture()
{
 D3DXMATRIX world_matrix, reflection_view_matrix, projection_matrix;
 bool result;

 reflection_texture_->Set_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view());

 reflection_texture_->Clear_render_target(d3d_->GetDeviceContext(), d3d_->Get_depth_stencil_view(), 0.0f, 0.0f, 0.0f, 1.0f);

 camera_->Render_reflection(water_height_);
 reflection_view_matrix = camera_->Get_reflection_view_matrix();

 d3d_->GetWorldMatrix(world_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
 bath_model_->Render(d3d_->GetDeviceContext());
 result = normal_shader_->Render(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, bath_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 
 d3d_->GetWorldMatrix(world_matrix);
 D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
 wall_model_->Render(d3d_->GetDeviceContext());
 normal_shader_->Render(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, wall_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 d3d_->GetWorldMatrix(world_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
 model_->Render(d3d_->GetDeviceContext());
 normal_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, reflection_view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());

 d3d_->Set_back_buffer_render_target();
 return true;
}


bool GraphicsClass::Render_scene()
{
 D3DXMATRIX world_matrix, view_matrix, projection_matrix, reflection_matrix;
 bool result;
 D3DXVECTOR3 scroll_speed, scale;
 D3DXVECTOR2 distortion1, distortion2, distortion3;
 float distortion_scale, distortion_bias;

 static float frame_time = 0.0f;
 frame_time += 0.01f;
 if (frame_time > 1000.0f)
  frame_time = 0.0f;

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


 D3DXMatrixTranslation(&world_matrix, 0.0f, 1.0f, 0.0f);
 ground_model_->Render(d3d_->GetDeviceContext());
 result = normal_shader_->Render(d3d_->GetDeviceContext(), ground_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, ground_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 if (!result)
  return false;
 d3d_->GetWorldMatrix(world_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, 6.0f, 8.0f);
 wall_model_->Render(d3d_->GetDeviceContext());
 result = normal_shader_->Render(d3d_->GetDeviceContext(), wall_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, wall_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 if (!result)
  return false;
 d3d_->GetWorldMatrix(world_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, 2.0f, 0.0f);
 bath_model_->Render(d3d_->GetDeviceContext());
 result = normal_shader_->Render(d3d_->GetDeviceContext(), bath_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, bath_model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 if (!result)
  return false;
 d3d_->GetWorldMatrix(world_matrix);



 reflection_matrix = camera_->Get_reflection_view_matrix();

 D3DXMatrixTranslation(&world_matrix, 0.0f, 5.0f, 0.0f);
 model_->Render(d3d_->GetDeviceContext());
 result = normal_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_textures(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 if (!result)
  return false;
 d3d_->GetWorldMatrix(world_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, water_height_ , 0.0f);
 water_model_->Render(d3d_->GetDeviceContext());
 result = water_shader_->Render(d3d_->GetDeviceContext(), water_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, reflection_matrix, reflection_texture_->Get_shader_resource_view(), refraction_texture_->Get_shader_resource_view(), water_model_->Get_texture(), water_translation_, 0.05f);
 if (!result)
  return false;
 d3d_->GetWorldMatrix(world_matrix);

 D3DXMatrixTranslation(&world_matrix, 0.0f, water_height_ + 5.0f, 7.0f);
 d3d_->TurnOnAlphaBlending();
 fire_model_->Render(d3d_->GetDeviceContext());
 result = fire_shader_->Render(d3d_->GetDeviceContext(), fire_model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, fire_model_->Get_textures()[0], fire_model_->Get_textures()[1], fire_model_->Get_textures()[2], frame_time, scroll_speed, scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
 if (!result)
  return false;
 d3d_->TurnOffAlphaBlending();

 d3d_->End_scene();
 return true;
}



