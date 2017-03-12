#include "particle_system_class.h"

ParticleSystemClass::ParticleSystemClass()
{
    texture_ = nullptr;
    particle_list_ = nullptr;
    vertices_ = nullptr;
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
}

ParticleSystemClass::ParticleSystemClass(const ParticleSystemClass&)
{
    
}

ParticleSystemClass::~ParticleSystemClass()
{
    
}

bool ParticleSystemClass::Initialize(ID3D11Device* device, WCHAR* texture_filename)
{
    bool result;
    result = Load_texture(device, texture_filename);
    if (!result)
        return false;

    result = Initialize_particle_system();
    if (!result)
        return false;

    result = Initialize_buffers(device);
    if (!result)
        return false;
    return true;
}

void ParticleSystemClass::Shutdown()
{
    Shutdown_buffers();
    Shutdown_particle_system();
    Release_texture();
}

bool ParticleSystemClass::Frame(float frame_time, ID3D11DeviceContext* device_context)
{
    bool result;
    Kill_particles();
    Emit_particles(frame_time);
    Update_particles(frame_time);

    result = Update_buffers(device_context);
    if (!result)
        return false;
    return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* device_context)
{
    Render_buffers(device_context);
}

ID3D11ShaderResourceView* ParticleSystemClass::Get_texture()
{
    return texture_->Get_texture();
}

int ParticleSystemClass::Get_index_count()
{
    return index_count_;
}

bool ParticleSystemClass::Load_texture(ID3D11Device* device, WCHAR* filename)
{
    bool result;

    texture_ = new TextureClass;
    if (!texture_)
        return false;

    result = texture_->Initialize(device, filename);
    if (!result)
        return false;
    return true;
}

void ParticleSystemClass::Release_texture()
{
    if(texture_)
    {
        texture_->Shutdown();
        delete texture_;
        texture_ = nullptr;
    }
}

bool ParticleSystemClass::Initialize_particle_system()
{
    int i;

    particle_deviation_x_ = 2.5f;
    particle_deviation_y_ = 0.1f;
    particle_deviation_z_ = 5.0f;

    particle_velocity_ = 1.0f;
    particle_velocity_variation_ = 2.2f;

    particle_size_ = 0.1f;
    particles_per_sec_ = 1000.0f;
    max_particles_ = 15000;

    particle_list_ = new PARTICLE_TYPE[max_particles_];
    if (!particle_list_)
        return false;

    for (i = 0; i < max_particles_; i++)
        particle_list_[i].active = false;

    current_particle_ = 0;
    accumulated_time_ = 0.0f;
    return true;
}

void ParticleSystemClass::Shutdown_particle_system()
{
    if(particle_list_)
    {
        delete[] particle_list_;
        particle_list_ = nullptr;
    }
}

bool ParticleSystemClass::Initialize_buffers(ID3D11Device* device)
{
    unsigned long* indices;
    int i;
    D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
    D3D11_SUBRESOURCE_DATA vertex_data, index_data;
    HRESULT result;

    vertex_count = max_particles_ * 6;
    index_count_ = vertex_count;

    vertices_ = new VERTEX_TYPE[vertex_count];
    if (!vertices_)
        return false;

    indices = new unsigned long[index_count_];
    if (!indices)
        return false;

    memset(vertices_, 0, (sizeof(VERTEX_TYPE)*vertex_count));
    for (i = 0; i < index_count_; i++)
        indices[i] = i;

    vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    vertex_buffer_desc.ByteWidth = sizeof(VERTEX_TYPE)*vertex_count;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertex_buffer_desc.MiscFlags = 0;
    vertex_buffer_desc.StructureByteStride = 0;

    vertex_data.pSysMem = vertices_;
    vertex_data.SysMemPitch = 0;
    vertex_data.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer_);
    if (FAILED(result))
        return false;

    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(unsigned long)*index_count_;
    index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buffer_desc.CPUAccessFlags = 0;
    index_buffer_desc.MiscFlags = 0;
    index_buffer_desc.StructureByteStride = 0;

    index_data.pSysMem = indices;
    index_data.SysMemPitch = 0;
    index_data.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer_);
    if (FAILED(result))
        return false;
    delete[] indices;
    indices = nullptr;
    return true;
}

void ParticleSystemClass::Shutdown_buffers()
{
    if(index_buffer_)
    {
        index_buffer_->Release();
        index_buffer_ = nullptr;
    }

    if(vertex_buffer_)
    {
        vertex_buffer_->Release();
        vertex_buffer_ = nullptr;
    }
}

void ParticleSystemClass::Emit_particles(float frame_time)
{
    bool emit_particle, found;
    float position_x, position_y, position_z, velocity, red, green, blue;
    int index, i, j;

    accumulated_time_ += frame_time;
    emit_particle = false;

    if(accumulated_time_ > (1000.0f / particles_per_sec_))
    {
        accumulated_time_ = 0.0f;
        emit_particle = true;
    }

    if ((emit_particle == true) && (current_particle_ < (max_particles_ - 1)))
    {
        current_particle_++;
        position_x = (((float)rand() - (float)rand()) / RAND_MAX) * particle_deviation_x_;
        position_y = (((float)rand() - (float)rand()) / RAND_MAX) * particle_deviation_y_;
        position_z = (((float)rand() - (float)rand()) / RAND_MAX) * particle_deviation_z_;


        velocity = particle_velocity_ + (((float)rand() - (float)rand()) / RAND_MAX) * particle_velocity_variation_;

        red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
        green = (((float)rand() - (float)rand()) / RAND_MAX);
        blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.2f;

        index = 0;
        found = false;
        while (!found)
        {
            if ((particle_list_[index].active == false) || (particle_list_[index].position_z < position_z))
                found = true;
            else
                index++;
        }

        i = current_particle_;
        j = i - 1;
        while (i != index)
        {
            particle_list_[i].position_x = particle_list_[j].position_x;
            particle_list_[i].position_y = particle_list_[j].position_y;
            particle_list_[i].position_z = particle_list_[j].position_z;

            particle_list_[i].red = particle_list_[j].red;
            particle_list_[i].green = particle_list_[j].green;
            particle_list_[i].blue = particle_list_[j].blue;

            particle_list_[i].velocity = particle_list_[j].velocity;
            particle_list_[i].active = particle_list_[j].active;
            i--;
            j--;
        }
        particle_list_[index].position_x = position_x;
        particle_list_[index].position_y = position_y;
        particle_list_[index].position_z = position_z;

        particle_list_[index].red = red;
        particle_list_[index].green = green;
        particle_list_[index].blue = blue;

        particle_list_[index].velocity = velocity;
        particle_list_[index].active = true;
    }    
}

void ParticleSystemClass::Update_particles(float frame_time)
{
    int i;
    for (i = 0; i < current_particle_; i++)
        particle_list_[i].position_y = particle_list_[i].position_y - (particle_list_[i].velocity * frame_time * 0.001f);
}

void ParticleSystemClass::Kill_particles()
{
    int i, j;
    for (i = 0; i < max_particles_; i++)
    {
        if((particle_list_[i].active == true) && (particle_list_[i].position_y < -3.0f))
        {
            particle_list_[i].active = false;
            current_particle_--;
            for (j = i; j < max_particles_ - 1; j++)
            {
                particle_list_[j].position_x = particle_list_[j + 1].position_x;
                particle_list_[j].position_y = particle_list_[j + 1].position_y;
                particle_list_[j].position_z = particle_list_[j + 1].position_z;

                particle_list_[j].red = particle_list_[j + 1].red;
                particle_list_[j].green = particle_list_[j + 1].green;
                particle_list_[j].blue = particle_list_[j + 1].blue;

                particle_list_[j].velocity = particle_list_[j + 1].velocity;
                particle_list_[j].active = particle_list_[j + 1].active;
            }
        }
    }
}

bool ParticleSystemClass::Update_buffers(ID3D11DeviceContext* device_context)
{
    int index, i;
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    VERTEX_TYPE* vertices_ptr;

    memset(vertices_, 0, (sizeof(VERTEX_TYPE)* vertex_count));
    index = 0;
    for (i = 0; i < current_particle_; i++)
    {
        //bottom left
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x - particle_size_, particle_list_[i].position_y - particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(0.0f, 1.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;

        //top left
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x - particle_size_, particle_list_[i].position_y + particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(0.0f, 0.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;

        //bottom right
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x + particle_size_, particle_list_[i].position_y - particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(1.0f, 1.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;

        //bottom right
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x + particle_size_, particle_list_[i].position_y - particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(1.0f, 1.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;
        
        //top left
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x - particle_size_, particle_list_[i].position_y + particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(0.0f, 0.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;

        //top right
        vertices_[index].position = D3DXVECTOR3(particle_list_[i].position_x + particle_size_, particle_list_[i].position_y + particle_size_, particle_list_[i].position_z);
        vertices_[index].texture = D3DXVECTOR2(1.0f, 0.0f);
        vertices_[index].color = D3DXVECTOR4(particle_list_[i].red, particle_list_[i].green, particle_list_[i].blue, 1.0f);
        index++;
    }

    result = device_context->Map(vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    if (FAILED(result))
        return false;

    vertices_ptr = (VERTEX_TYPE*)mapped_resource.pData;
    memcpy(vertices_ptr, (void*)vertices_, (sizeof(VERTEX_TYPE) * vertex_count));
    device_context->Unmap(vertex_buffer_, 0);
    return true;
}

void ParticleSystemClass::Render_buffers(ID3D11DeviceContext* device_context)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VERTEX_TYPE);
    offset = 0;

    device_context->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
    device_context->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
    device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
