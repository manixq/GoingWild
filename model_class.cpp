#include "model_class.h"

ModelClass::ModelClass()
{
 vertex_buffer_ = 0;
 index_buffer_ = 0;
 texture_ = 0;
}

ModelClass::ModelClass(const ModelClass&)
{
 
}

ModelClass::~ModelClass()
{
 
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* texture_filename)
{
 bool result;

 //initialize the vertex and index buffer that hold the geometry for the triangle
 result = Initialize_buffers(device);
 if (!result)
  return false;

 //load the texture
 result = Load_texture(device, texture_filename);
 if (!result)
  return false;
 return true;
}

void ModelClass::Shutdown()
{
 Release_texture();
 Shutdown_buffers();
}

void ModelClass::Render(ID3D11DeviceContext* device_context)
{
 Render_buffers(device_context);
}

int ModelClass::Get_index_count()
{
 return index_count_;
}

ID3D11ShaderResourceView* ModelClass::Get_texture()
{
 return texture_->Get_texture();
}

bool ModelClass::Initialize_buffers(ID3D11Device* device)
{
 VERTEX_TYPE* vertices;
 unsigned long* indices;
 D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
 D3D11_SUBRESOURCE_DATA vertex_data, index_data;
 HRESULT result;

 vertex_count_ = 3;
 index_count_ = 3;

 vertices = new VERTEX_TYPE[vertex_count_];
 if (!vertices)
  return false;

 indices = new unsigned long[index_count_];
 if (!indices)
  return false;

 //load the vertex array with data
 vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); //bottom left
 vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
 vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

 vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
 vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
 vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

 vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
 vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
 vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

 // Load the index array with data.
 indices[0] = 0;  // Bottom left.
 indices[1] = 1;  // Top middle.
 indices[2] = 2;  // Bottom right.

 //setup description
 vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
 vertex_buffer_desc.ByteWidth = sizeof(VERTEX_TYPE) * vertex_count_;
 vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
 vertex_buffer_desc.CPUAccessFlags = 0;
 vertex_buffer_desc.MiscFlags = 0;
 vertex_buffer_desc.StructureByteStride = 0;

 vertex_data.pSysMem = vertices;
 vertex_data.SysMemPitch = 0;
 vertex_data.SysMemSlicePitch = 0;

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
 //index_data.SysMemPitch = 0;
 //index_data.SysMemSlicePitch = 0;

 result = device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer_);
 if (FAILED(result))
  return false;

 delete[] vertices;
 vertices = nullptr;

 delete[] indices;
 indices = nullptr;

 return true;
}

void ModelClass::Shutdown_buffers()
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

void ModelClass::Render_buffers(ID3D11DeviceContext* device_context)
{
 unsigned int stride;
 unsigned int offset;

 stride = sizeof(VERTEX_TYPE);
 offset = 0;

 device_context->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
 device_context->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
 device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::Load_texture(ID3D11Device* device, WCHAR* filename)
{
 bool result;

 //create texture object
 texture_ = new TextureClass;
 if (!texture_)
  return false;

 result = texture_->Initialize(device, filename);
 if (!result)
  return false;

 return true;
}

void ModelClass::Release_texture()
{
 if (texture_)
 {
  texture_->Shutdown();
  texture_ = nullptr;
 }
}






