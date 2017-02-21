#include "graphics_class.h"

GraphicsClass::GraphicsClass()
{
 d3d_ = nullptr;
 camera_ = nullptr;
 model_ = nullptr;
 multitexture_shader_ = nullptr;
 light_ = nullptr;
 model_list_ = nullptr;
 frustum_ = nullptr;
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

 //create camera object
 camera_ = new CameraClass;
 if (!camera_)
  return false;

 //init pos of camera
 camera_->Set_position(0.0f, 0.0f, -10.0f);

 //create model object
 model_ = new ModelClass;
 if (!model_)
  return false;

 //init model
 result = model_->Initialize(d3d_->GetDevice(), "../Engine/data/sphere.txt" ,L"../Engine/data/floor.dds", L"../Engine/data/floor_mix.dds");
 if (!result)
 {
  MessageBox(hwnd, L"Could not initialize the model object.", L"error", MB_OK);
  return false;
 }

 //create color shader object
 multitexture_shader_ = new MultitextureShaderClass;
 if (!multitexture_shader_)
  return false;

 result = multitexture_shader_->Initialize(d3d_->GetDevice(), hwnd);
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

 frustum_ = new FrustumClass;
 if (!frustum_)
  return false;

 light_->Set_ambient_color(0.15f, 0.15f, 0.15f, 1.0f);
 light_->Set_diffuse_color(1.0f, 1.0f, 1.0f, 1.0f);
 light_->Set_direction(0.0f, 0.0f, 1.0f);
 light_->Set_specular_color(1.0f, 1.0f, 1.0f, 1.0f);
 light_->Set_specular_power(32.0f);

 return true;
}

//kill all graphics objects
void GraphicsClass::Shutdown()
{
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

 if(multitexture_shader_)
 {
  multitexture_shader_->Shutdown();
  delete multitexture_shader_;
  multitexture_shader_ = nullptr;
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

bool GraphicsClass::Frame(float rotation_y)
{
// bool result;

 camera_->Set_position(0.0f, 0.0f, -5.0f);
 camera_->Set_rotation(0.0f, rotation_y, 0.0f);
 /*static float rotation = 0.0f;
 rotation += static_cast<float>(D3DX_PI * 0.005f);
 if (rotation > 360.0f)
  rotation -= 360.0f;
 result = Render(rotation);*/

// if (!result)
 // return false;

 return true;
}

bool GraphicsClass::Render()
{
 D3DXMATRIX view_matrix, projection_matrix, world_matrix, ortho_matrix;
 int model_count, render_count, index;
 float position_x, position_y, position_z, radius;
 D3DXVECTOR4 color;
 bool render_model, result;

 d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);
 camera_->Render();
 d3d_->GetWorldMatrix(world_matrix);
 camera_->Get_view_matrix(view_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);
 d3d_->GetOrthoMatrix(ortho_matrix);

 frustum_->ConstructFrustrum(SCREEN_DEPTH, projection_matrix, view_matrix);
 model_count = model_list_->Get_model_count();
 render_count = 0;

 for (index = 0; index < model_count; index++)
 {
  model_list_->Get_data(index, position_x, position_y, position_z, color);
  radius = 1.0f;
  render_model = frustum_->Check_sphere(position_x, position_y, position_z, radius);
  if(render_model)
  {
   D3DXMatrixTranslation(&world_matrix, position_x, position_y, position_z);
   model_->Render(d3d_->GetDeviceContext());
   result = multitexture_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_texture(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
   if (!result)
    return false;

   d3d_->GetWorldMatrix(world_matrix);
   render_count++;
  }
 }
 //present on screen
 d3d_->End_scene();
 return true;
}



