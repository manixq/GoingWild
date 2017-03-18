#include "terrain_class.h"

TerrainClass::TerrainClass()
{
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
    terrain_filename_ = nullptr;
    height_map_ = nullptr;
    terrain_model_ = nullptr;
}

TerrainClass::TerrainClass(const TerrainClass&)
{
    
}

TerrainClass::~TerrainClass()
{
    
}

bool TerrainClass::Initialize(ID3D11Device* device, char* setup_filename)
{
    bool result;

    result = Load_setup_file(setup_filename);
    if (!result)
        return false;

    result = Load_bitmap_height_map();
    if (!result)
        return false;

    Set_terrain_coordinates();

    result = Calculate_normals();
    if (!result)
        return false;

    result = Build_terrain_model();
    if (!result)
        return false;

    Shutdown_height_map();

    result = Initialize_buffer(device);
    if (!result)
        return false;

    Shutdown_terrain_model();

    return true;
}

void TerrainClass::Shutdown()
{
    Shutdown_buffers();
    Shutdown_terrain_model();
    Shutdown_height_map();
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

bool TerrainClass::Load_setup_file(char* filename)
{
    int string_len;
    ifstream fin;
    char input;

    string_len = 256;
    terrain_filename_ = new char[string_len];
    if (!terrain_filename_)
        return false;

    fin.open(filename);
    if (fin.fail())
        return false;

    fin.get(input);
    while (input != ':')
        fin.get(input);
    fin >> terrain_filename_;

    fin.get(input);
    while (input != ':')
        fin.get(input);
    fin >> terrain_height_;

    fin.get(input);
    while (input != ':')
        fin.get(input);
    fin >> terrain_width_;

    fin.get(input);
    while (input != ':')
        fin.get(input);
    fin >> height_scale_;

    fin.close();
    return true;
}

bool TerrainClass::Load_bitmap_height_map()
{
    int error, image_size, i, j, k, index;
    FILE* file_ptr;
    unsigned long long count;
    BITMAPFILEHEADER bitmap_file_header;
    BITMAPINFOHEADER bitmap_info_header;
    unsigned char* bitmap_image;
    unsigned char height;

    height_map_ = new HEIGHT_MAP_TYPE[terrain_width_ * terrain_height_];
    if (!height_map_)
        return false;

    error = fopen_s(&file_ptr, terrain_filename_, "rb");
    if (error != 0)
        return false;

    count = fread(&bitmap_file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr);
    if (count != 1)
        return false;

    count = fread(&bitmap_info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr);
    if (count != 1)
        return false;

    if ((bitmap_info_header.biHeight != terrain_height_) || (bitmap_info_header.biWidth != terrain_width_))
        return false;

    //need to add extra coz 257x257 non-divide-able
    image_size = terrain_height_ * ((terrain_width_ * 3) + 1);

    bitmap_image = new unsigned char[image_size];
    if (!bitmap_image)
        return false;

    //beginning of bitmap data
    fseek(file_ptr, bitmap_file_header.bfOffBits, SEEK_SET);
    count = fread(bitmap_image, 1, image_size, file_ptr);
    if (count != image_size)
        return false;

    error = fclose(file_ptr);
    if (error != 0)
        return false;

    k = 0;
    for (j = 0; j < terrain_height_; j++)
    {
        for (i = 0; i < terrain_width_; i++)
        {
            index = (terrain_width_ * (terrain_height_ - 1 - j)) + i;
            height = bitmap_image[k];
            height_map_[index].y = (float)height;
            k += 3;
        }
k++;
    }
    delete[] bitmap_image;
    bitmap_image = nullptr;

    delete[] terrain_filename_;
    terrain_filename_ = nullptr;

    return true;
}

void TerrainClass::Shutdown_height_map()
{
    if (height_map_)
    {
        delete[] height_map_;
        height_map_ = nullptr;
    }
}

//bitmap is upside down
void TerrainClass::Set_terrain_coordinates()
{
    int i, j, index;

    for (j = 0; j < terrain_height_; j++)
    {
        for (i = 0; i < terrain_width_; i++)
        {
            index = (terrain_width_ * j) + i;

            height_map_[index].x = (float)i;
            height_map_[index].z = -(float)j;
            height_map_[index].z += (float)(terrain_height_ - 1);
            height_map_[index].y /= height_scale_;

        }
    }
}

bool TerrainClass::Calculate_normals()
{
    int i, j, index1, index2, index3, index;
    float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
    VECTOR_TYPE* normals;

    normals = new VECTOR_TYPE[(terrain_height_ - 1) * (terrain_width_ - 1)];
    if (!normals)
        return false;

    for (j = 0; j < (terrain_height_ - 1); j++)
    {
        for (i = 0; i < (terrain_width_ - 1); i++)
        {
            index1 = ((j + 1) * terrain_width_) + i;
            index2 = ((j + 1) * terrain_width_) + (i + 1);
            index3 = ((j)* terrain_width_) + i;

            vertex1[0] = height_map_[index1].x;
            vertex1[1] = height_map_[index1].y;
            vertex1[2] = height_map_[index1].z;

            vertex2[0] = height_map_[index2].x;
            vertex2[1] = height_map_[index2].y;
            vertex2[2] = height_map_[index2].z;

            vertex3[0] = height_map_[index3].x;
            vertex3[1] = height_map_[index3].y;
            vertex3[2] = height_map_[index3].z;

            vector1[0] = vertex1[0] - vertex3[0];
            vector1[1] = vertex1[1] - vertex3[1];
            vector1[2] = vertex1[2] - vertex3[2];
            vector2[0] = vertex3[0] - vertex2[0];
            vector2[1] = vertex3[1] - vertex2[1];
            vector2[2] = vertex3[2] - vertex2[2];

            index = (j * (terrain_width_ - 1)) + i;

            normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
            normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
            normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

            length = (float)sqrt((normals[index].x * normals[index].x) + (normals[index].y * normals[index].y) + (normals[index].z * normals[index].z));

            normals[index].x = (normals[index].x / length);
            normals[index].y = (normals[index].y / length);
            normals[index].z = (normals[index].z / length);
        }
    }

    for (j = 0; j < terrain_width_; j++)
    {
        for (i = 0; i < terrain_width_; i++)
        {
            sum[0] = 0.0f;
            sum[1] = 0.0f;
            sum[2] = 0.0f;

            if (((i - 1) >= 0) && ((j - 1) >= 0))
            {
                index = ((j - 1) * (terrain_width_ - 1)) + (i - 1);
                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
            }

            if ((i < (terrain_width_ - 1)) && ((j - 1) >= 0))
            {
                index = ((j - 1) * (terrain_width_ - 1)) + i;
                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
            }

            if (((i - 1) >= 0) && (j < terrain_height_ - 1))
            {
                index = (j * (terrain_width_ - 1)) + (i - 1);
                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
            }

            if ((i < (terrain_width_ - 1)) && (j < terrain_height_ - 1))
            {
                index = (j * (terrain_width_ - 1)) + i;
                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
            }

            length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));
            index = (j * terrain_width_) + i;

            height_map_[index].nx = (sum[0] / length);
            height_map_[index].ny = (sum[1] / length);
            height_map_[index].nz = (sum[2] / length);
        }
    }
    delete[] normals;
    normals = nullptr;

    return true;
}

bool TerrainClass::Build_terrain_model()
{
    int i, j, index, index1, index2, index3, index4;

    vertex_count_ = (terrain_height_ - 1) * (terrain_width_ - 1) * 6;

    terrain_model_ = new MODEL_TYPE[vertex_count_];
    if (!terrain_model_)
        return false;

    //triangle strip
    index = 0;
    for (j = 0; j < (terrain_height_ - 1); j++)
    {
        for (i = 0; i < (terrain_width_ - 1); i++)
        {
            index1 = (terrain_width_ * j) + i;
            index2 = (terrain_width_ * j) + (i + 1);
            index3 = (terrain_width_ * (j + 1)) + i;
            index4 = (terrain_width_ * (j + 1)) + (i + 1);

            terrain_model_[index].x = height_map_[index1].x;
            terrain_model_[index].y = height_map_[index1].y;
            terrain_model_[index].z = height_map_[index1].z;
            terrain_model_[index].tu = 0.0f;
            terrain_model_[index].tv = 0.0f;
            terrain_model_[index].nx = height_map_[index1].nx;
            terrain_model_[index].ny = height_map_[index1].ny;
            terrain_model_[index].nz = height_map_[index1].nz;
            index++;

            terrain_model_[index].x = height_map_[index2].x;
            terrain_model_[index].y = height_map_[index2].y;
            terrain_model_[index].z = height_map_[index2].z;
            terrain_model_[index].tu = 1.0f;
            terrain_model_[index].tv = 0.0f; 
            terrain_model_[index].nx = height_map_[index2].nx;
            terrain_model_[index].ny = height_map_[index2].ny;
            terrain_model_[index].nz = height_map_[index2].nz;
            index++;

            terrain_model_[index].x = height_map_[index3].x;
            terrain_model_[index].y = height_map_[index3].y;
            terrain_model_[index].z = height_map_[index3].z;
            terrain_model_[index].tu = 0.0f;
            terrain_model_[index].tv = 1.0f;
            terrain_model_[index].nx = height_map_[index3].nx;
            terrain_model_[index].ny = height_map_[index3].ny;
            terrain_model_[index].nz = height_map_[index3].nz;
            index++;

            terrain_model_[index].x = height_map_[index3].x;
            terrain_model_[index].y = height_map_[index3].y;
            terrain_model_[index].z = height_map_[index3].z;
            terrain_model_[index].tu = 0.0f;
            terrain_model_[index].tv = 1.0f;
            terrain_model_[index].nx = height_map_[index3].nx;
            terrain_model_[index].ny = height_map_[index3].ny;
            terrain_model_[index].nz = height_map_[index3].nz;
            index++;

            terrain_model_[index].x = height_map_[index2].x;
            terrain_model_[index].y = height_map_[index2].y;
            terrain_model_[index].z = height_map_[index2].z;
            terrain_model_[index].tu = 1.0f;
            terrain_model_[index].tv = 0.0f;
            terrain_model_[index].nx = height_map_[index2].nx;
            terrain_model_[index].ny = height_map_[index2].ny;
            terrain_model_[index].nz = height_map_[index2].nz;
            index++;

            terrain_model_[index].x = height_map_[index4].x;
            terrain_model_[index].y = height_map_[index4].y;
            terrain_model_[index].z = height_map_[index4].z;
            terrain_model_[index].tu = 1.0f;
            terrain_model_[index].tv = 1.0f;
            terrain_model_[index].nx = height_map_[index4].nx;
            terrain_model_[index].ny = height_map_[index4].ny;
            terrain_model_[index].nz = height_map_[index4].nz;
            index++;
        }
    }
    return true;
}

void TerrainClass::Shutdown_terrain_model()
{
    if(terrain_model_)
    {
        delete[] terrain_model_;
        terrain_model_ = nullptr;
    }
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
    vertex_count_ = (terrain_width - 1) * (terrain_height - 1) * 6;

    index_count_ = vertex_count_;

    vertices = new VERTEX_TYPE[vertex_count_];
    if (!vertices)
        return false;

    indices = new unsigned long[index_count_];
    if (!indices)
        return false;

    index = 0;

    for (i = 0; i < vertex_count_; i++)
    {
        vertices[i].position = XMFLOAT3(terrain_model_[i].x, terrain_model_[i].y, terrain_model_[i].z);
        vertices[i].texture = XMFLOAT2(terrain_model_[i].tu, terrain_model_[i].tv);
        vertices[i].normal = XMFLOAT3(terrain_model_[i].nx, terrain_model_[i].ny, terrain_model_[i].nz);
        indices[i] = i;
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
    device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
