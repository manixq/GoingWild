#include "terrain_class.h"

TerrainClass::TerrainClass()
{
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
}

TerrainClass::TerrainClass(const TerrainClass&)
{
    
}

TerrainClass::~TerrainClass()
{
    
}

bool TerrainClass::Initialize(ID3D11Device* device)
{
    bool result;

    result = Initialize_buffer(device);
    if (!result)
        return false;

    return true;
}

void TerrainClass::Shutdown()
{
    Shutdown_buffers();
}

bool TerrainClass::Render(ID3D11DeviceContext* device_context)
{
    Render_buffers(device_context);
    return true;
}

int TerrainClass::Get_index_count()
{
    return index_count_;
}

bool TerrainClass::Initialize_buffer(ID3D11Device* device)
{
    VERTEX_TYPE* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
    D3D11_SUBRESOURCE_DATA vertex_data, index_data;
    HRESULT result;
    int i, j, terrain_width, terrain_height, index;
    XMFLOAT4 color;
    float position_x, position_z;

    terrain_height = 256;
    terrain_width = 256;

    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex_count_ = (terrain_width - 1) * (terrain_height - 1) * 8;

    index_count_ = vertex_count_;

    vertices = new VERTEX_TYPE[vertex_count_];
    if (!vertices)
        return false;

    indices = new unsigned long[index_count_];
    if (!indices)
        return false;

    index = 0;

    for (j = 0; j < (terrain_height - 1); j++)
    {
        for (i = 0; i < (terrain_width - 1); i++)
        {
            position_x = (float)i;
            position_z = (float)(j + 1);

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)(i + 1);
            position_z = (float)(j + 1);

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)(i + 1);
            position_z = (float)(j + 1);

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)(i + 1);
            position_z = (float)j;

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)(i + 1);
            position_z = (float)j;

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)i;
            position_z = (float)j;

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)i;
            position_z = (float)j;

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;

            position_x = (float)i;
            position_z = (float)(j + 1);

            vertices[index].position = XMFLOAT3(position_x, 0.0f, position_z);
            vertices[index].color = color;
            indices[index] = index;
            index++;
        }
    }

    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(VERTEX_TYPE) * vertex_count_;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;
    vertex_buffer_desc.StructureByteStride = 0;

    vertex_data.pSysMem = vertices;
    vertex_data.SysMemSlicePitch = 0;
    vertex_data.SysMemPitch = 0;

    result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer_);
    if (FAILED(result))
        return false;

    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(unsigned long) * index_count_;
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

    delete[] vertices;
    vertices = nullptr;
    delete[] indices;
    indices = nullptr;
    return true;
}

void TerrainClass::Shutdown_buffers()
{
    if (index_buffer_)
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

void TerrainClass::Render_buffers(ID3D11DeviceContext* device_context)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VERTEX_TYPE);
    offset = 0;

    device_context->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
    device_context->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
    device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
