#include "fire_shader_class.h"

FireShaderClass::FireShaderClass()
{
 vertex_shader_ = nullptr;
 pixel_shader_ = nullptr;
 layout_ = nullptr;
 matrix_buffer_ = nullptr;
 noise_buffer_ = nullptr;
 sample_state_ = nullptr;
 sample_state2_ = nullptr;
 distortion_buffer_ = nullptr;
}

FireShaderClass::FireShaderClass(const FireShaderClass&)
{

}

FireShaderClass::~FireShaderClass()
{

}

bool FireShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
 bool result;
 result = Initialize_shader(device, hwnd, L"../Engine/shaders/vertex/fire.vs", L"../Engine/shaders/pixel/fire.ps");
 if (!result)
  return false;
 return true;
}

void FireShaderClass::Shutdown()
{
 Shutdown_shader();
}

bool FireShaderClass::Render(ID3D11DeviceContext* device_context, int index_count, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* fire_texture, ID3D11ShaderResourceView* noise_texture, ID3D11ShaderResourceView* alpha_texture, float frame_time, D3DXVECTOR3 scroll_speed, D3DXVECTOR3 scale, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3, float distortion_scale, float distortion_bias)
{
 bool result;
 result = Set_shader_parameters(device_context, world, view, projection, fire_texture, noise_texture, alpha_texture, frame_time, scroll_speed, scale, distortion1, distortion2, distortion3, distortion_scale, distortion_bias);
 if (!result)
  return false;

 Render_shader(device_context, index_count);
 return true;
}

bool FireShaderClass::Initialize_shader(ID3D11Device* device, HWND hwnd, WCHAR* vs_filename, WCHAR* ps_filename)
{
 HRESULT result;
 ID3D10Blob* error_message;
 ID3D10Blob* vertex_shader_buffer;
 ID3D10Blob* pixel_shader_buffer;
 D3D11_INPUT_ELEMENT_DESC polygon_layout[2];
 unsigned int num_elements;
 D3D11_BUFFER_DESC matrix_buffer_desc;
 D3D11_BUFFER_DESC noise_buffer_desc;
 D3D11_SAMPLER_DESC sampler_desc;
 D3D11_SAMPLER_DESC sampler_desc2;
 D3D11_BUFFER_DESC distortion_buffer_desc;

 error_message = nullptr;
 vertex_shader_buffer = nullptr;
 pixel_shader_buffer = nullptr;

 result = D3DX11CompileFromFile(vs_filename, nullptr, nullptr, "Fire_vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertex_shader_buffer, &error_message, nullptr);
 if (FAILED(result))
 {
  if (error_message)
   Output_shader_error_message(error_message, hwnd, vs_filename);
  else
   MessageBox(hwnd, vs_filename, L"Missing shader file", MB_OK);
  return false;
 }

 result = D3DX11CompileFromFile(ps_filename, nullptr, nullptr, "Fire_pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixel_shader_buffer, &error_message, nullptr);
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

 num_elements = sizeof(polygon_layout) / sizeof(polygon_layout[0]);

 result = device->CreateInputLayout(polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &layout_);
 if (FAILED(result))
  return false;

 vertex_shader_buffer->Release();
 vertex_shader_buffer = nullptr;

 pixel_shader_buffer->Release();
 pixel_shader_buffer = nullptr;

 matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 matrix_buffer_desc.ByteWidth = sizeof(MATRIX_BUFFER_TYPE);
 matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 matrix_buffer_desc.MiscFlags = 0;
 matrix_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&matrix_buffer_desc, nullptr, &matrix_buffer_);
 if (FAILED(result))
  return false;

 noise_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 noise_buffer_desc.ByteWidth = sizeof(NOISE_BUFFER_TYPE);
 noise_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 noise_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 noise_buffer_desc.MiscFlags = 0;
 noise_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&noise_buffer_desc, nullptr, &noise_buffer_);
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

 sampler_desc2.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
 sampler_desc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
 sampler_desc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
 sampler_desc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
 sampler_desc2.MipLODBias = 0.0f;
 sampler_desc2.MaxAnisotropy = 1;
 sampler_desc2.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
 sampler_desc2.BorderColor[0] = 0;
 sampler_desc2.BorderColor[1] = 0;
 sampler_desc2.BorderColor[2] = 0;
 sampler_desc2.BorderColor[3] = 0;
 sampler_desc2.MinLOD = 0;
 sampler_desc2.MaxLOD = D3D11_FLOAT32_MAX;

 result = device->CreateSamplerState(&sampler_desc2, &sample_state2_);
 if (FAILED(result))
  return false;

 distortion_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 distortion_buffer_desc.ByteWidth = sizeof(DISTORTION_BUFFER_TYPE);
 distortion_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 distortion_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 distortion_buffer_desc.MiscFlags = 0;
 distortion_buffer_desc.StructureByteStride = 0;

 result = device->CreateBuffer(&distortion_buffer_desc, nullptr, &distortion_buffer_);
 if (FAILED(result))
  return false;

 return true;
}

void FireShaderClass::Shutdown_shader()
{
 if(noise_buffer_)
 {
  noise_buffer_->Release();
  noise_buffer_ = nullptr;
 }

 if(distortion_buffer_)
 {
  distortion_buffer_->Release();
  distortion_buffer_ = nullptr;
 }

 if (sample_state2_)
 {
  sample_state2_->Release();
  sample_state2_ = nullptr;
 }

 if (sample_state_)
 {
  sample_state_->Release();
  sample_state_ = nullptr;
 }

 if (matrix_buffer_)
 {
  matrix_buffer_->Release();
  matrix_buffer_ = nullptr;
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

void FireShaderClass::Output_shader_error_message(ID3D10Blob* error_message, HWND hwnd, WCHAR* shader_filename)
{
 char* compile_errors;
 unsigned long buffer_size, i;
 ofstream fout;

 compile_errors = static_cast<char*>(error_message->GetBufferPointer());
 buffer_size = error_message->GetBufferSize();
 fout.open("shader_error.txt");
 for (i = 0; i < buffer_size; i++)
  fout << compile_errors[i];
 fout.close();

 error_message->Release();
 error_message = nullptr;

 MessageBox(hwnd, L"Error compiling shader. check txt file.", shader_filename, MB_OK);
}

bool FireShaderClass::Set_shader_parameters(ID3D11DeviceContext* device_context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* fire_texture, ID3D11ShaderResourceView* noise_texture, ID3D11ShaderResourceView* alpha_texture, float frame_time, D3DXVECTOR3 scroll_speed, D3DXVECTOR3 scale, D3DXVECTOR2 distortion1, D3DXVECTOR2 distortion2, D3DXVECTOR2 distortion3, float distortion_scale, float distortion_bias)
{
 HRESULT result;
 D3D11_MAPPED_SUBRESOURCE mapped_resource;
 MATRIX_BUFFER_TYPE* data_ptr;
 NOISE_BUFFER_TYPE* data_ptr2;
 DISTORTION_BUFFER_TYPE* data_ptr3;
 unsigned int buffer_number;

 D3DXMatrixTranspose(&world, &world);
 D3DXMatrixTranspose(&view, &view);
 D3DXMatrixTranspose(&projection, &projection);

 //lock the const buffer
 result = device_context->Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr = static_cast<MATRIX_BUFFER_TYPE*>(mapped_resource.pData);

 data_ptr->world = world;
 data_ptr->view = view;
 data_ptr->projection = projection;

 //unlock
 device_context->Unmap(matrix_buffer_, 0);

 //set pos of constant buffer in vertexshader
 buffer_number = 0;

 //set constatnt buffer in vertex shader with updated valuse
 device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer_);

 result = device_context->Map(noise_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;
 data_ptr2 = (NOISE_BUFFER_TYPE*)mapped_resource.pData;
 data_ptr2->frame_time = frame_time;
 data_ptr2->scroll_speed = scroll_speed;
 data_ptr2->scale = scale;
 data_ptr2->padding = 0.0f;

 device_context->Unmap(noise_buffer_, 0);
 buffer_number = 1;

 device_context->VSSetConstantBuffers(buffer_number, 1, &noise_buffer_);

 device_context->PSSetShaderResources(0, 1, &fire_texture);
 device_context->PSSetShaderResources(1, 1, &noise_texture);
 device_context->PSSetShaderResources(2, 1, &alpha_texture);

 result = device_context->Map(distortion_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
 if (FAILED(result))
  return false;

 data_ptr3 = (DISTORTION_BUFFER_TYPE*)mapped_resource.pData;
 data_ptr3->distortion1 = distortion1;
 data_ptr3->distortion2 = distortion2;
 data_ptr3->distortion3 = distortion3;
 data_ptr3->distortion_scale = distortion_scale;
 data_ptr3->distortion_bias = distortion_bias;

 device_context->Unmap(distortion_buffer_, 0);
 buffer_number = 0;
 //set shader texture resource in the pixel shader
 device_context->PSSetConstantBuffers(buffer_number, 1, &distortion_buffer_);

 return true;
}

void FireShaderClass::Render_shader(ID3D11DeviceContext* device_context, int index_count)
{
 //set vertex input layout
 device_context->IASetInputLayout(layout_);

 //set vs and ps that ll be used to render
 device_context->VSSetShader(vertex_shader_, nullptr, 0);
 device_context->PSSetShader(pixel_shader_, nullptr, 0);

 //set sampler state in ps
 device_context->PSSetSamplers(0, 1, &sample_state_);
 device_context->PSSetSamplers(0, 1, &sample_state2_);

 //render
 device_context->DrawIndexed(index_count, 0, 0);
}