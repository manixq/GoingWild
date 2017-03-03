#include "water_shader_class.h"

WaterShaderClass::WaterShaderClass()
{
 vertex_shader_ = nullptr;
 pixel_shader_ = nullptr;
 layout_ = nullptr;
 sampler_state_ = nullptr;
 matrix_buffer_ = nullptr;
 reflectiion_buffer_ = nullptr;
 water_buffer_ = nullptr;
}

WaterShaderClass::WaterShaderClass(const WaterShaderClass& other)
{
 
}

WaterShaderClass::~WaterShaderClass()
{
 
}

bool WaterShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
 bool result;

 result = Initialize_shader(device, hwnd, L"../Engine/shaders/vertex/water.vs", L"../Engine/shaders/pixel/water.ps");
 if (!result)
  return false;

 return true;
}

void WaterShaderClass::Shutdown()
{
 ShutdownShader();
}

bool WaterShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, D3DXMATRIX world_matrix, D3DXMATRIX view_matrix, D3DXMATRIX projection_matrix, D3DXMATRIX reflection_matrix, ID3D11ShaderResourceView* reflection_texture, ID3D11ShaderResourceView* refraction_texture, ID3D11ShaderResourceView* normal_texture, float water_translation, float reflect_refract_scale)
{
 bool result;
 result = Set_shader_parameters(device_context, world_matrix, view_matrix, projection_matrix, reflection_matrix, reflection_texture, refraction_texture, normal_texture, water_translation, reflect_refract_scale);
 if (!result)
  return false;

 Render_shader(device_context, index_count);
 return true;
}

bool WaterShaderClass::Initialize_shader(ID3D11Device* device, HWND hwnd, WCHAR* vs_filename, WCHAR* ps_filename)
{
 HRESULT result;
 ID3D10Blob* error_message;
 ID3D10Blob* vertex_shader_buffer;
 ID3D10Blob* pixel_shader_buffer;
 D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
 unsigned int num_elements;
 D3D11_SAMPLER_DESC sampler_desc;
 D3D11_BUFFER_DESC matrix_buffer_desc;
 D3D11_BUFFER_DESC reflection_buffer_desc;
 D3D11_BUFFER_DESC water_buffer_desc;

 error_message = 0;
 vertex_shader_buffer = 0;
 pixel_shader_buffer = 0;

 result = D3DX11CompileFromFile(vs_filename, nullptr, nullptr, "Water_vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertex_shader_buffer, &error_message, nullptr);
 if(FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, vs_filename);
  else
   MessageBox(hwnd, vs_filename, L"missing shader file", MB_OK);
  return false;
 }


 result = D3DX11CompileFromFile(ps_filename, nullptr, nullptr, "Water_pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixel_shader_buffer, &error_message, nullptr);
 if (FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, ps_filename);
  else
   MessageBox(hwnd, ps_filename, L"missing shader file", MB_OK);
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
 result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &layout_);
 if (FAILED(result))
  return false;

 pixel_shader_buffer->Release();
 pixel_shader_buffer = 0;

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

 result = device->CreateSamplerState(&sampler_desc, &sampler_state_);
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

 reflection_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 reflection_buffer_desc.ByteWidth = sizeof(REFLECTION_BUFFER_TYPE);
 reflection_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 reflection_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 reflection_buffer_desc.MiscFlags = 0;
 reflection_buffer_desc.StructureByteStride = 0;
 
 result = device->CreateBuffer(&reflection_buffer_desc, nullptr, &reflectiion_buffer_);
 if (FAILED(result))
  return false;

 water_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 water_buffer_desc.ByteWidth = sizeof(WATER_BUFFER_TYPE);
 water_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 water_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 water_buffer_desc.MiscFlags = 0;
 water_buffer_desc.StructureByteStride = 0;
 
 result = device->CreateBuffer(&water_buffer_desc, nullptr, &water_buffer_);
 if (FAILED(result))
  return false;

 return true;
}

void WaterShaderClass::ShutdownShader()
{
 if(water_buffer_)
 {
  water_buffer_->Release();
  water_buffer_ = nullptr;
 }

 if(reflectiion_buffer_)
 {
  reflectiion_buffer_->Release();
  reflectiion_buffer_ = nullptr;
 }

 if(matrix_buffer_)
 {
  matrix_buffer_->Release();
  matrix_buffer_ = nullptr;
 }

 if(sampler_state_)
 {
  sampler_state_->Release();
  sampler_state_ = nullptr;
 }

 if(layout_)
 {
  layout_->Release();
  layout_ = nullptr;
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

void WaterShaderClass::Output_shader_error_message(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_filename)
{
 char* compile_errors;
 unsigned long buffer_size, i;
 ofstream fout;

 compile_errors = (char*)(error_message->GetBufferPointer());

 buffer_size = error_message->GetBufferSize();

 fout.open("shader-error.txt");
 for (i = 0; i < buffer_size; i++)
  fout << compile_errors[i];
 fout.close();

 error_message->Release();
 error_message = 0;

 MessageBox(hwnd, L"Error compiling shader. check text file.", shader_filename, MB_OK);
}

bool WaterShaderClass::Set_shader_parameters(ID3D11DeviceContext* device_context, D3DXMATRIX world_matrix, D3DXMATRIX view_matrix, D3DXMATRIX projection_matrix, D3DXMATRIX reflection_matrix, ID3D11ShaderResourceView* reflection_texture, ID3D11ShaderResourceView* refraction_texture, ID3D11ShaderResourceView* normal_texture, float water_translation, float reflect_refract_scale)
{
 HRESULT result;
 D3D11_MAPPED_SUBRESOURCE mapped_resource;
 unsigned int buffer_number;
 MATRIX_BUFFER_TYPE* data_ptr;
 REFLECTION_BUFFER_TYPE* data_ptr2;
 WATER_BUFFER_TYPE* data_ptr3;

 D3DXMatrixTranspose(&world_matrix, &world_matrix);
 D3DXMatrixTranspose(&view_matrix, &view_matrix);
 D3DXMatrixTranspose(&projection_matrix, &projection_matrix);
 D3DXMatrixTranspose(&reflection_matrix, &reflection_matrix);

 //set regular matrix in vs
 result = device_context->Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if(FAILED(result))
  return false;

 data_ptr = (MATRIX_BUFFER_TYPE*)mapped_resource.pData;
 data_ptr->world = world_matrix;
 data_ptr->view = view_matrix;
 data_ptr->projection = projection_matrix;

 device_context->Unmap(matrix_buffer_, 0);

 buffer_number = 0;

 device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer_);

 //set reflection mx buffer in vs
 result = device_context->Map(reflectiion_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;
 data_ptr2 = (REFLECTION_BUFFER_TYPE*)mapped_resource.pData;
 data_ptr2->reflection = reflection_matrix;

 device_context->Unmap(reflectiion_buffer_, 0);

 buffer_number = 1;

 device_context->VSSetConstantBuffers(buffer_number, 1, &reflectiion_buffer_);

 //set reflection refraction and normal map txts in ps
 device_context->PSSetShaderResources(0, 1, &reflection_texture);
 device_context->PSSetShaderResources(1, 1, &refraction_texture);
 device_context->PSSetShaderResources(2, 1, &normal_texture);

 result = device_context->Map(water_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr3 = (WATER_BUFFER_TYPE*)mapped_resource.pData;
 data_ptr3->water_translation = water_translation;
 data_ptr3->reflect_refract_scale = reflect_refract_scale;
 data_ptr3->padding = D3DXVECTOR2(0.0f, 0.0f);

 device_context->Unmap(water_buffer_, 0);

 buffer_number = 0;

 device_context->PSSetConstantBuffers(buffer_number, 1, &water_buffer_);

 return true;
}

void WaterShaderClass::Render_shader(ID3D11DeviceContext* device_context, int index_count)
{
 device_context->IASetInputLayout(layout_);
 device_context->VSSetShader(vertex_shader_, nullptr, 0);
 device_context->PSSetShader(pixel_shader_, nullptr, 0);
 device_context->DrawIndexed(index_count, 0, 0);
}
