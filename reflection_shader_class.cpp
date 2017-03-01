#include "reflection_shader_class.h"

ReflectionShaderClass::ReflectionShaderClass()
{
 vertex_shader_ = 0;
 pixel_shader_ = 0;
 input_layout_ = 0;
 matrix_buffer_ = 0;
 sample_state_ = 0;
 reflection_buffer_ = 0;
}

ReflectionShaderClass::ReflectionShaderClass(const ReflectionShaderClass&)
{
 
}

ReflectionShaderClass::~ReflectionShaderClass()
{
 
}

bool ReflectionShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
 bool result;

 result = Initialize_shader(device, hwnd, L"../Engine/shaders/vertex/reflection.vs", L"../Engine/shaders/pixel/reflection.ps");
 if (!result)
  return false;

 return true;
}

void ReflectionShaderClass::Shutdown()
{
 Shutdown_shader();
}

bool ReflectionShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, D3DXMATRIX world_matrix,
 D3DXMATRIX view_matrix, D3DXMATRIX projection_matrix, ID3D11ShaderResourceView* texture,
 ID3D11ShaderResourceView* reflection_texture, D3DXMATRIX reflection_matrix)
{
 bool result;

 result = Set_shader_parameters(device_context, world_matrix, view_matrix, projection_matrix, texture,
  reflection_texture, reflection_matrix);
 if (!result)
  return false;

 Render_shader(device_context, index_count);
 return true;
}

bool ReflectionShaderClass::Initialize_shader(ID3D11Device* device, HWND hwnd, WCHAR* vs_filename, WCHAR* ps_filename)
{
 HRESULT result;
 ID3D10Blob* error_message;
 ID3D10Blob* vertex_shader_buffer;
 ID3D10Blob* pixel_shader_buffer;
 D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
 unsigned int num_elements;
 D3D11_BUFFER_DESC matrix_buffer_desc;
 D3D11_SAMPLER_DESC sampler_desc;
 D3D11_BUFFER_DESC reflection_buffer_desc;

 error_message = 0;
 vertex_shader_buffer = 0;
 pixel_shader_buffer = 0;

 result = D3DX11CompileFromFile(vs_filename, nullptr, nullptr, "Reflection_vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
  0, nullptr, &vertex_shader_buffer, &error_message, nullptr);
 if(FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, vs_filename);
  else
   MessageBox(hwnd, vs_filename, L"Missing reflection shader file", MB_OK);
  return false;
 }

 result = D3DX11CompileFromFile(ps_filename, nullptr, nullptr, "Reflection_pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
  0, nullptr, &pixel_shader_buffer, &error_message, nullptr);

 if(FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, ps_filename);
  else
   MessageBox(hwnd, ps_filename, L"Missing reflection shader file", MB_OK);
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

 num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

 result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &input_layout_);
 if (FAILED(result))
  return false;

 vertex_shader_buffer->Release();
 vertex_shader_buffer = 0;

 pixel_shader_buffer->Release();
 pixel_shader_buffer = 0;

 matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 matrix_buffer_desc.ByteWidth = sizeof(MATRIXBUFFERTYPE);
 matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 matrix_buffer_desc.MiscFlags = 0;
 matrix_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&matrix_buffer_desc, nullptr, &matrix_buffer_);
 if (FAILED(result))
  return false;

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

 reflection_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 reflection_buffer_desc.ByteWidth = sizeof(REFLECTIONBUFFERTYPE);
 reflection_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 reflection_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 reflection_buffer_desc.MiscFlags = 0;
 reflection_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&reflection_buffer_desc, nullptr, &reflection_buffer_);
 if (FAILED(result))
  return false;
 return true;
}

void ReflectionShaderClass::Shutdown_shader()
{
 if(reflection_buffer_)
 {
  reflection_buffer_->Release();
  reflection_buffer_ = nullptr;
 }

 if(sample_state_)
 {
  sample_state_->Release();
  sample_state_ = nullptr;
 }

 if(matrix_buffer_)
 {
  matrix_buffer_->Release();
  matrix_buffer_ = nullptr;
 }

 if(input_layout_)
 {
  input_layout_->Release();
  input_layout_ = nullptr;
 }

 if(pixel_shader_)
 {
  pixel_shader_->Release();
  pixel_shader_ = nullptr;
 }
 if(vertex_shader_)
 {
  vertex_shader_->Release();
  vertex_shader_ = nullptr;
 }
}

void ReflectionShaderClass::Output_shader_error_message(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_filename)
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

bool ReflectionShaderClass::Set_shader_parameters(ID3D11DeviceContext* device_context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* reflection_texture, D3DXMATRIX reflection_matrix)
{
 HRESULT result;
 D3D11_MAPPED_SUBRESOURCE mapped_resource;
 MATRIXBUFFERTYPE* data_ptr;
 unsigned int buffer_number;
 REFLECTIONBUFFERTYPE* data_ptr2;

 D3DXMatrixTranspose(&world, &world);
 D3DXMatrixTranspose(&view, &view);
 D3DXMatrixTranspose(&projection, &projection);

 D3DXMatrixTranspose(&reflection_matrix, &reflection_matrix);

 result = device_context->Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr = (MATRIXBUFFERTYPE*)mapped_resource.pData;

 data_ptr->world = world;
 data_ptr->view = view;
 data_ptr->projection = projection;

 device_context->Unmap(matrix_buffer_, 0);

 buffer_number = 0;

 device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer_);

 result = device_context->Map(reflection_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr2 = (REFLECTIONBUFFERTYPE*)mapped_resource.pData;
 data_ptr2->reflection_matrix = reflection_matrix;
 device_context->Unmap(reflection_buffer_, 0);
 buffer_number = 1;
 device_context->VSSetConstantBuffers(buffer_number, 1, &reflection_buffer_);
 device_context->PSSetShaderResources(0, 1, &texture);
 device_context->PSSetShaderResources(1, 1, &reflection_texture);

 return true;
}

void ReflectionShaderClass::Render_shader(ID3D11DeviceContext* device_context, int index_count)
{
 device_context->IASetInputLayout(input_layout_);

 device_context->VSSetShader(vertex_shader_, nullptr, 0);
 device_context->PSSetShader(pixel_shader_, nullptr, 0);

 device_context->PSSetSamplers(0, 1, &sample_state_);

 device_context->DrawIndexed(index_count, 0, 0);
}
