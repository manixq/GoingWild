#ifndef _GRAPHICS_CLASS_H_
#define _GRAPHICS_CLASS_H_

//========
//includes
#include "d3d_class.h"
#include "camera_class.h"
#include "model_class.h"
#include "light_class.h"
#include "model_list_class.h"
#include "debug_window_class.h"
#include "render_texture_class.h"
#include "ortho_window_class.h"
#include "particle_system_class.h"
#include "bitmap_class.h"
#include "deferred_buffers_class.h"
#include "text_class.h"
#include "terrain_class.h"
#include "texture_manager_class.h"
#include "skybox_class.h"
#include "shader_manager_class.h"

//=======
//globals
static bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f; 
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

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
    bool Frame(float, float, float, float, float);
    bool Render();
    bool Handle_input(bool, int, int);

private:
    void Test_intersection();
    bool Ray_sphere_intersect(D3DXVECTOR3, D3DXVECTOR3, float);
    
    bool Render_scene();
    bool Render_light_scene_to_texture();
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
    DebugWindowClass* debug_window_;
    OrthoWindowClass *small_window_, *full_sceen_window_;
    LightClass* light_;

    ParticleSystemClass* particle_system_;
    ModelListClass* model_list_;

    ModelClass* model_;
    ModelClass *ground_model_, *wall_model_, *bath_model_, *water_model_;
    ModelClass* floor_model_;
    ModelClass* fire_model_;
    TerrainClass* terrain_;
    SkyBoxClass* skybox_;

    TextClass* text_;
    TextureManagerClass* texture_manager_;
    ShaderManagerClass* shader_manager_;

    RenderTextureClass *reflection_texture_, *refraction_texture_;
    RenderTextureClass *render_texture_, *down_sample_texture_, *horizontal_blur_texture_, *vertical_blur_texture_, *up_sample_texture_;
    RenderTextureClass *shadow_texture_;

    DeferredBuffersClass* deferred_buffers_;

    BitmapClass* mouse_;
    bool begin_check_;
    int mouse_x_ = 0;
    int mouse_y_ = 0;

    float water_height_, water_translation_;
    float rotation_x_ = 0.0f;
    float rotation_y_ = 0.0f;
    float frame_time_ = 0.0f;

    float screen_width_;
    float screen_height_;

    bool fire_on_ = false;
};
#endif
