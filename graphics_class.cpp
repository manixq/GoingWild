#include "graphics_class.h"

GraphicsClass::GraphicsClass()
{
 d3d_ = nullptr;
 camera_ = nullptr;
 model_ = nullptr;
 light_shader_ = nullptr;
 light_ = nullptr;
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
 result = model_->Initialize(d3d_->GetDevice(), "../Engine/data/cube.txt" ,L"../Engine/data/floor.dds");
 if (!result)
 {
  MessageBox(hwnd, L"Could not initialize the model object.", L"error", MB_OK);
  return false;
 }

 //create color shader object
 light_shader_ = new LightShaderClass;
 if (!light_shader_)
  return false;

 result = light_shader_->Initialize(d3d_->GetDevice(), hwnd);
 if(!result)
 {
  MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
  return false;
 }

 light_ = new LightClass;
 if (!light_)
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
 if (light_)
 {
  delete light_;
  light_ = nullptr;
 }

 if(light_shader_)
 {
  light_shader_->Shutdown();
  delete light_shader_;
  light_shader_ = nullptr;
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

bool GraphicsClass::Frame()
{
 bool result;

 static float rotation = 0.0f;
 rotation += static_cast<float>(D3DX_PI * 0.005f);
 if (rotation > 360.0f)
  rotation -= 360.0f;
 result = Render(rotation);

 if (!result)
  return false;

 return true;
}

bool GraphicsClass::Render(float rotation)
{
 D3DXMATRIX view_matrix, projection_matrix, world_matrix;
 bool result;

 d3d_->Begin_scene(0.0f, 0.0f, 0.0f, 1.0f);
 camera_->Render();
 camera_->Get_view_matrix(view_matrix);
 d3d_->GetWorldMatrix(world_matrix);
 d3d_->GetProjectionMatrix(projection_matrix);

 D3DXMatrixRotationY(&world_matrix, rotation);
 model_->Render(d3d_->GetDeviceContext());

 result = light_shader_->Render(d3d_->GetDeviceContext(), model_->Get_index_count(), world_matrix, view_matrix, projection_matrix, model_->Get_texture(), light_->Get_direction(), light_->Get_ambient_color(), light_->Get_diffuse_color(), camera_->Get_position(), light_->Get_specular_color(), light_->Get_specular_power());
 if (!result)
  return false;

 //present on screen
 d3d_->End_scene();
 return true;
}



