#ifndef _GRAPHICS_CLASS_H_
#define _GRAPHICS_CLASS_H_

//========
//includes
#include "d3d_class.h"
#include "camera_class.h"
#include "model_class.h"
#include "normal_shader_class.h"
#include "light_class.h"
#include "model_list_class.h"
#include "frustum_class.h"


//=======
//globals
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//==============
//GraphicsClass
class GraphicsClass
{
public:
 GraphicsClass();
 GraphicsClass(const GraphicsClass&);
 ~GraphicsClass();

 bool Initialize(int, int, HWND);
 void Shutdown();
 bool Frame(float);
 bool Render();

private:
 D3DClass* d3d_;
 CameraClass* camera_;
 ModelClass* model_;
 NormalShaderClass* normal_shader_;
 LightClass* light_;
 ModelListClass* model_list_;
 FrustumClass* frustum_;
};

#endif
