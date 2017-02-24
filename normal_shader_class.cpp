#include "normal_shader_class.h"

NormalShaderClass::NormalShaderClass()
{
 vertex_shader_ = nullptr;
 pixel_shader_ = nullptr;
 layout_ = nullptr;
 sample_state_ = nullptr;
 matrix_buffer_ = nullptr;
 camera_buffer_ = nullptr;
 light_buffer_ = nullptr;
 clip_plane_buffer_ = nullptr;
}

NormalShaderClass::NormalShaderClass(const NormalShaderClass& other)
{

}

NormalShaderClass::~NormalShaderClass()
{

}

bool NormalShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
 bool result;

 result = Initialize_shader(device, hwnd, L"../Engine/shaders/vertex/normal_map.vs", L"../Engine/shaders/pixel/normal_map.ps");
 if (!result)
  return false;
 return true;
}

void NormalShaderClass::Shutdown()
{
 Shutdown_shader();
}

bool NormalShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** texture, D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color, D3DXVECTOR3 camera_position, D3DXVECTOR4 specular_color, float specular_power, D3DXVECTOR4 clip_plane)
{
 bool result;

 result = Set_shader_parameters(device_context, world, view, projection, texture, light_direction, ambient_color, diffuse_color, camera_position, specular_color, specular_power, clip_plane);
 if (!result)
  return false;

 Render_shader(device_context, index_count);
 return true;
}

bool NormalShaderClass::Initialize_shader(ID3D11Device* device, HWND hwnd, WCHAR* vs_filename, WCHAR* ps_filename)
{
 HRESULT result;
 ID3D10Blob* error_message;
 ID3D10Blob* vertex_shader_buffer;
 ID3D10Blob* pixel_shader_buffer;

 D3D11_INPUT_ELEMENT_DESC polygon_layout[5];
 unsigned int num_elements;
 D3D11_SAMPLER_DESC sampler_desc;
 D3D11_BUFFER_DESC matrix_buffer_desc;
 D3D11_BUFFER_DESC camera_buffer_desc;
 D3D11_BUFFER_DESC light_buffer_desc;
 D3D11_BUFFER_DESC clip_plane_buffer_desc;

 error_message = nullptr;
 vertex_shader_buffer = nullptr;
 pixel_shader_buffer = nullptr;

 result = D3DX11CompileFromFile(vs_filename, nullptr, nullptr, "Normal_vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertex_shader_buffer, &error_message, nullptr);
 if (FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, vs_filename);
  else
   MessageBox(hwnd, vs_filename, L"Missing Shader File", MB_OK);
  return false;
 }

 result = D3DX11CompileFromFile(ps_filename, nullptr, nullptr, "Normal_pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixel_shader_buffer, &error_message, nullptr);
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

 //setup to match vertextype struct in modelclass and in shader
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

 //tangent and binormal elements
 polygon_layout[3].SemanticName = "TANGENT";
 polygon_layout[3].SemanticIndex = 0;
 polygon_layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
 polygon_layout[3].InputSlot = 0;
 polygon_layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
 polygon_layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
 polygon_layout[3].InstanceDataStepRate = 0;

 polygon_layout[4].SemanticName = "BINORMAL";
 polygon_layout[4].SemanticIndex = 0;
 polygon_layout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
 polygon_layout[4].InputSlot = 0;
 polygon_layout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
 polygon_layout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
 polygon_layout[4].InstanceDataStepRate = 0;

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

 result = device->CreateSamplerState(&sampler_desc, &sample_state_);
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

 camera_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 camera_buffer_desc.ByteWidth = sizeof(CAMERA_BUFFER_TYPE);
 camera_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 camera_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 camera_buffer_desc.MiscFlags = 0;
 camera_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&camera_buffer_desc, nullptr, &camera_buffer_);
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

 clip_plane_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 clip_plane_buffer_desc.ByteWidth = sizeof(CLIP_PLANE_BUFFER_TYPE);
 clip_plane_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 clip_plane_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 clip_plane_buffer_desc.MiscFlags = 0;
 clip_plane_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&clip_plane_buffer_desc, nullptr, &clip_plane_buffer_);
 if (FAILED(result))
  return false;

 return true;
}

void NormalShaderClass::Shutdown_shader()
{
 if(clip_plane_buffer_)
 {
  clip_plane_buffer_->Release();
  clip_plane_buffer_ = nullptr;
 }

 if (light_buffer_)
 {
  light_buffer_->Release();
  light_buffer_ = nullptr;
 }

 if (camera_buffer_)
 {
  camera_buffer_->Release();
  camera_buffer_ = nullptr;
 }

 if (matrix_buffer_)
 {
  matrix_buffer_->Release();
  matrix_buffer_ = nullptr;
 }

 if (sample_state_)
 {
  sample_state_->Release();
  sample_state_ = nullptr;
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

void NormalShaderClass::Output_shader_error_message(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_filename)
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

bool NormalShaderClass::Set_shader_parameters(ID3D11DeviceContext* device_context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** texture_array, D3DXVECTOR3 light_direction, D3DXVECTOR4 ambient_color, D3DXVECTOR4 diffuse_color, D3DXVECTOR3 camera_position, D3DXVECTOR4 specular_color, float specular_power, D3DXVECTOR4 clip_plane)
{
 HRESULT result;
 D3D11_MAPPED_SUBRESOURCE mapped_resource;
 unsigned int buffer_number;
 MATRIX_BUFFER_TYPE* data_ptr;
 LIGHT_BUFFER_TYPE* data_ptr2;
 CAMERA_BUFFER_TYPE* data_ptr3;
 CLIP_PLANE_BUFFER_TYPE* data_ptr4;

 D3DXMatrixTranspose(&world, &world);
 D3DXMatrixTranspose(&view, &view);
 D3DXMatrixTranspose(&projection, &projection);

 result = device_context->Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr = static_cast<MATRIX_BUFFER_TYPE*>(mapped_resource.pData);
 data_ptr->world = world;
 data_ptr->view = view;
 data_ptr->projection = projection;

 device_context->Unmap(matrix_buffer_, 0);
 buffer_number = 0;

 device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer_);

 result = device_context->Map(camera_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr3 = static_cast<CAMERA_BUFFER_TYPE*>(mapped_resource.pData);
 data_ptr3->camera_position = camera_position;
 data_ptr3->padding = 0.0f;

 device_context->Unmap(camera_buffer_, 0);

 buffer_number = 1;
 device_context->VSSetConstantBuffers(buffer_number, 1, &camera_buffer_);

 device_context->PSSetShaderResources(0, 3, texture_array);

 result = device_context->Map(light_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr2 = static_cast<LIGHT_BUFFER_TYPE*>(mapped_resource.pData);
 data_ptr2->ambient_color = ambient_color;
 data_ptr2->diffuse_color = diffuse_color;
 data_ptr2->light_direction = light_direction;
 data_ptr2->specular_color = specular_color;
 data_ptr2->specular_power = specular_power;

 device_context->Unmap(light_buffer_, 0);
 buffer_number = 0;

 device_context->PSSetConstantBuffers(buffer_number, 1, &light_buffer_);

 result = device_context->Map(clip_plane_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;
 data_ptr4 = static_cast<CLIP_PLANE_BUFFER_TYPE*>(mapped_resource.pData);
 data_ptr4->clip_plane = clip_plane;
 device_context->Unmap(clip_plane_buffer_, 0);
 buffer_number = 2;
 device_context->VSSetConstantBuffers(buffer_number, 1, &clip_plane_buffer_);
 return true;
}

void NormalShaderClass::Render_shader(ID3D11DeviceContext* device_context, int index_count)
{
 device_context->IASetInputLayout(layout_);

 device_context->VSSetShader(vertex_shader_, nullptr, 0);
 device_context->PSSetShader(pixel_shader_, nullptr, 0);

 device_context->PSSetSamplers(0, 1, &sample_state_);

 device_context->DrawIndexed(index_count, 0, 0);
}