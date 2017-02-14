#include "graphics_class.h"

GraphicsClass::GraphicsClass()
{
 d3d_ = nullptr;
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
 return true;
}

//kill all graphics objects
void GraphicsClass::Shutdown()
{
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

 //render func each frame
 result = Render();
 if (!result)
  return false;

 return true;
}

bool GraphicsClass::Render()
{
 d3d_->Begin_scene(0.5f, 0.5f, 0.5f, 1.0f);

 //present on screen
 d3d_->End_scene();
 return true;
}



