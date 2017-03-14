#include "shadow_shader_class.h"


ShadowShaderClass::ShadowShaderClass()
{
    vertex_shader_ = nullptr;
    pixel_shader_ = nullptr;
    layout_ = nullptr;
    sample_state_wrap_ = nullptr;
    sample_state_clamp_ = nullptr;
    matrix_buffer_ = nullptr;
    light_buffer_ = nullptr;
    light_buffer2_ = nullptr;
}

ShadowShaderClass::ShadowShaderClass(const ShadowShaderClass&)
{

}

ShadowShaderClass::~ShadowShaderClass()
{

}

bool ShadowShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;

    result = Initialize_shader(device, hwnd, L"../Engine/shaders/vertex/shadow.vs", L"../Engine/shaders/pixel/shadow.ps");
    if (!result)
        return false;
    return true;
}

void ShadowShaderClass::Shutdown()
{
    Shutdown_shader();
}

bool ShadowShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX light_view, D3DXMATRIX light_projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depth_map_texture, D3DXVECTOR3 light_position, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color)
{
    bool result;

    result = Set_shader_parameters(device_context, world, view, projection, light_view, light_projection, texture, depth_map_texture, light_position, ambient_color, diffuse_color);
    if (!result)
        return false;

    Render_shader(device_context, index_count);
    return true;
}

bool ShadowShaderClass::Initialize_shader(ID3D11Device* device, HWND hwnd, WCHAR* vs_filename, WCHAR* ps_filename)
{
    HRESULT result;
    ID3D10Blob* error_message;
    ID3D10Blob* vertex_shader_buffer;
    ID3D10Blob* pixel_shader_buffer;

    D3D11_INPUT_ELEMENT_DESC polygon_layout[3];
    unsigned int num_elements;
    D3D11_SAMPLER_DESC sampler_desc;
    D3D11_BUFFER_DESC matrix_buffer_desc;
    D3D11_BUFFER_DESC light_buffer_desc;
    D3D11_BUFFER_DESC light_buffer_desc2;

    error_message = nullptr;
    vertex_shader_buffer = nullptr;
    pixel_shader_buffer = nullptr;

    result = D3DX11CompileFromFile(vs_filename, nullptr, nullptr, "Shadow_vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertex_shader_buffer, &error_message, nullptr);
    if (FAILED(result))
    {
        if (error_message)
            Output_shader_error_message(error_message, hwnd, vs_filename);
        else
            MessageBox(hwnd, vs_filename, L"Missing Shader File", MB_OK);
        return false;
    }

    result = D3DX11CompileFromFile(ps_filename, nullptr, nullptr, "Shadow_pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixel_shader_buffer, &error_message, nullptr);
    if (FAILED(result))
    {
        if (error_message)
            Output_shader_error_message(error_message, hwnd, ps_filename);
        else
            MessageBox(hwnd, ps_filename, L"Missing shader file", MB_OK);
        return false;
    }
    result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), nullptr, &vertex_shader_);
    if (FAILED(result))
        return false;

    result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), nullptr, &pixel_shader_);
    if (FAILED(result))
        return false;

    polygon_layout[0].SemanticName = "POSITION";
    polygon_layout[0].SemanticIndex = 0;
    polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygon_layout[0].InputSlot = 0;
    polygon_layout[0].AlignedByteOffset = 0;
    polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygon_layout[0].InstanceDataStepRate = 0;

    polygon_layout[1].SemanticName = "TEXCOORD";
    polygon_layout[1].SemanticIndex = 0;
    polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygon_layout[1].InputSlot = 0;
    polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygon_layout[1].InstanceDataStepRate = 0;

    polygon_layout[2].SemanticName = "NORMAL";
    polygon_layout[2].SemanticIndex = 0;
    polygon_layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygon_layout[2].InputSlot = 0;
    polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygon_layout[2].InstanceDataStepRate = 0;

    num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

    result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &layout_);
    if (FAILED(result))
        return false;

    vertex_shader_buffer->Release();
    vertex_shader_buffer = nullptr;

    pixel_shader_buffer->Release();
    pixel_shader_buffer = nullptr;

    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.MipLODBias = 0.0f;
    sampler_desc.MaxAnisotropy = 1;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampler_desc.BorderColor[0] = 0;
    sampler_desc.BorderColor[1] = 0;
    sampler_desc.BorderColor[2] = 0;
    sampler_desc.BorderColor[3] = 0;
    sampler_desc.MinLOD = 0;
    sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

    result = device->CreateSamplerState(&sampler_desc, &sample_state_wrap_);
    if (FAILED(result))
        return false;

    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    result = device->CreateSamplerState(&sampler_desc, &sample_state_clamp_);
    if (FAILED(result))
        return false;

    matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    matrix_buffer_desc.ByteWidth = sizeof(MATRIX_BUFFER_TYPE);
    matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrix_buffer_desc.MiscFlags = 0;
    matrix_buffer_desc.StructureByteStride = 0;

    result = device->CreateBuffer(&matrix_buffer_desc, nullptr, &matrix_buffer_);
    if (FAILED(result))
        return false;

    light_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    light_buffer_desc.ByteWidth = sizeof(LIGHT_BUFFER_TYPE);
    light_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    light_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    light_buffer_desc.MiscFlags = 0;
    light_buffer_desc.StructureByteStride = 0;

    result = device->CreateBuffer(&light_buffer_desc, nullptr, &light_buffer_);
    if (FAILED(result))
        return false;

    light_buffer_desc2.Usage = D3D11_USAGE_DYNAMIC;
    light_buffer_desc2.ByteWidth = sizeof(LIGHT_BUFFER_TYPE2);
    light_buffer_desc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    light_buffer_desc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    light_buffer_desc2.MiscFlags = 0;
    light_buffer_desc2.StructureByteStride = 0;

    result = device->CreateBuffer(&light_buffer_desc2, nullptr, &light_buffer2_);
    if (FAILED(result))
        return false;

    return true;
}

void ShadowShaderClass::Shutdown_shader()
{
    if (light_buffer_)
    {
        light_buffer_->Release();
        light_buffer_ = nullptr;
    }

    if (light_buffer2_)
    {
        light_buffer2_->Release();
        light_buffer2_ = nullptr;
    }

    if (matrix_buffer_)
    {
        matrix_buffer_->Release();
        matrix_buffer_ = nullptr;
    }

    if (sample_state_wrap_)
    {
        sample_state_wrap_->Release();
        sample_state_wrap_ = nullptr;
    }

    if (sample_state_clamp_)
    {
        sample_state_clamp_->Release();
        sample_state_clamp_ = nullptr;
    }

    if (layout_)
    {
        layout_->Release();
        layout_ = nullptr;
    }

    if (pixel_shader_)
    {
        pixel_shader_->Release();
        pixel_shader_ = nullptr;
    }

    if (vertex_shader_)
    {
        vertex_shader_->Release();
        vertex_shader_ = nullptr;
    }
}

void ShadowShaderClass::Output_shader_error_message(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_filename)
{
    char* compile_error;
    unsigned long buffer_size, i;
    ofstream fout;

    compile_error = static_cast<char*>(error_message->GetBufferPointer());
    buffer_size = error_message->GetBufferSize();
    fout.open("shader_error.txt");
    for (i = 0; i<buffer_size; i++)
    {
        fout << compile_error[i];
    }
    fout.close();
    error_message->Release();
    error_message = nullptr;
    MessageBox(hwnd, L"Error compiling shader", shader_filename, MB_OK);
}

bool ShadowShaderClass::Set_shader_parameters(ID3D11DeviceContext* device_context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX light_view, D3DXMATRIX light_projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depth_map_texture, D3DXVECTOR3 light_position, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    unsigned int buffer_number;
    MATRIX_BUFFER_TYPE* data_ptr;
    LIGHT_BUFFER_TYPE* data_ptr2;
    LIGHT_BUFFER_TYPE2* data_ptr3;

    D3DXMatrixTranspose(&world, &world);
    D3DXMatrixTranspose(&view, &view);
    D3DXMatrixTranspose(&projection, &projection);
    D3DXMatrixTranspose(&light_view, &light_view);
    D3DXMatrixTranspose(&light_projection, &light_projection);

    result = device_context->Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    if (FAILED(result))
        return false;

    data_ptr = static_cast<MATRIX_BUFFER_TYPE*>(mapped_resource.pData);
    data_ptr->world = world;
    data_ptr->view = view;
    data_ptr->projection = projection;
    data_ptr->light_view = light_view;
    data_ptr->light_projection = light_projection;
    
    device_context->Unmap(matrix_buffer_, 0);
    buffer_number = 0;

    device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer_);
    device_context->PSSetShaderResources(0, 1, &texture);
    device_context->PSSetShaderResources(1, 1, &depth_map_texture);

    result = device_context->Map(light_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    if (FAILED(result))
        return false;

    data_ptr2 = static_cast<LIGHT_BUFFER_TYPE*>(mapped_resource.pData);
    data_ptr2->ambient_color = ambient_color;
    data_ptr2->diffuse_color = diffuse_color;

    device_context->Unmap(light_buffer_, 0);
    buffer_number = 0;

    device_context->PSSetConstantBuffers(buffer_number, 1, &light_buffer_);

    result = device_context->Map(light_buffer2_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    if (FAILED(result))
        return false;

    data_ptr3 = static_cast<LIGHT_BUFFER_TYPE2*>(mapped_resource.pData);
    data_ptr3->light_position = light_position;
    data_ptr3->padding = 0.0f;

    device_context->Unmap(light_buffer2_, 0);
    buffer_number = 1;

    device_context->VSSetConstantBuffers(buffer_number, 1, &light_buffer2_);

    return true;
}

void ShadowShaderClass::Render_shader(ID3D11DeviceContext* device_context, int index_count)
{
    device_context->IASetInputLayout(layout_);

    device_context->VSSetShader(vertex_shader_, nullptr, 0);
    device_context->PSSetShader(pixel_shader_, nullptr, 0);

    device_context->PSSetSamplers(0, 1, &sample_state_clamp_);
    device_context->PSSetSamplers(1, 1, &sample_state_wrap_);

    device_context->DrawIndexed(index_count, 0, 0);
}