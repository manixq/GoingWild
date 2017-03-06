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
#include "debug_window_class.h"
#include "render_texture_class.h"
#include "reflection_shader_class.h"
#include "refraction_shader_class.h"
#include "water_shader_class.h"
#include "fire_shader_class.h"

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
 bool Frame(float, float, float, float);
 bool Render();

private:
 D3DClass* d3d_;
 CameraClass* camera_;
 ModelClass* model_;
 ModelClass *ground_model_, *wall_model_, *bath_model_, *water_model_;
 NormalShaderClass* normal_shader_;
 FireShaderClass* fire_shader_;
 LightClass* light_;
 ModelListClass* model_list_;
 FrustumClass* frustum_;
 ModelClass* floor_model_;
 ModelClass* fire_model_;
 DebugWindowClass* debug_window_;
 RenderTextureClass *reflection_texture_, *refraction_texture_;

 ReflectionShaderClass* reflection_shader_;
 RefractionShaderClass* refraction_shader_;
 WaterShaderClass* water_shader_;

 bool Render_scene();
 bool Render_to_texture();
 bool Render_refraction_to_texture();
 bool Render_reflection_to_texture();

 float water_height_, water_translation_;

};

#endif
