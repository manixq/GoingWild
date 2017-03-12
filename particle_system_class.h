#ifndef _PARTICLE_SYSTEM_CLASS_H_
#define _PARTICLE_SYSTEM_CLASS_H_

//========
//Includes
#include <d3d11.h>
#include <D3DX10math.h>
#include "texture_class.h"

//===================
//ParticleSystemclass
class ParticleSystemClass
{
    struct PARTICLE_TYPE
    {
        float position_x, position_y, position_z;
        float red, green, blue;
        float velocity;
        bool active;
    };

    struct VERTEX_TYPE
    {
        D3DXVECTOR3 position;
        D3DXVECTOR2 texture;
        D3DXVECTOR4 color;
    };

public:
    ParticleSystemClass();
    ParticleSystemClass(const ParticleSystemClass&);
    ~ParticleSystemClass();

    bool Initialize(ID3D11Device*, WCHAR*);
    void Shutdown();
    bool Frame(float, ID3D11DeviceContext*);
    void Render(ID3D11DeviceContext*);

    ID3D11ShaderResourceView* Get_texture();
    int Get_index_count();

private:
    bool Load_texture(ID3D11Device*, WCHAR*);
    void Release_texture();

    bool Initialize_particle_system();
    void Shutdown_particle_system();

    bool Initialize_buffers(ID3D11Device*);
    void Shutdown_buffers();

    void Emit_particles(float);
    void Update_particles(float);
    void Kill_particles();

    bool Update_buffers(ID3D11DeviceContext*);
    void Render_buffers(ID3D11DeviceContext*);

    float particle_deviation_x_, particle_deviation_y_, particle_deviation_z_;
    float particle_velocity_, particle_velocity_variation_;
    float particle_size_, particles_per_sec_;
    float max_particles_;

    int current_particle_;
    float accumulated_time_;

    TextureClass* texture_;
    PARTICLE_TYPE* particle_list_;

    int vertex_count, index_count_;
    VERTEX_TYPE* vertices_;
    ID3D11Buffer *vertex_buffer_, *index_buffer_;
};
#endif