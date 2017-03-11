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
#include "depth_shader_class.h"
#include "horizontal_blur_shader_class.h"
#include "vertical_blur_shader_class.h"
#include "ortho_window_class.h"
#include "texture_shader_class.h"
#include "color_shader_class.h"

//=======
//globals
static bool FULL_SCREEN = true;
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
    bool Render_scene();
    bool Render_scene_to_texture();
    bool Render_horizontal_bloor_to_texture();
    bool Render_vertical_bloor_to_texture();
    bool Up_sample_texture();
    bool Down_sample_texture();
    bool Render2d_texture_scene();
    bool Render_refraction_to_texture();
    bool Render_reflection_to_texture();

    D3DClass* d3d_;
    CameraClass* camera_;
    ModelClass* model_;
    ModelClass *ground_model_, *wall_model_, *bath_model_, *water_model_;
    ColorShaderClass* color_shader_;
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
    DepthShaderClass* depth_shader_;
    HorizontalBlurShaderClass* horizontal_blur_shader_;
    VerticalBlurShaderClass* vertical_blur_shader_;
    RenderTextureClass *render_texture_, *down_sample_texture_, *horizontal_blur_texture_, *vertical_blur_texture_, *up_sample_texture_;
    OrthoWindowClass *small_window_, *full_sceen_window_;
    float water_height_, water_translation_;
    TextureShaderClass* texture_shader_;

    float rotation_x_ = 0.0f;
    float rotation_y_ = 0.0f;  
};
#endif
