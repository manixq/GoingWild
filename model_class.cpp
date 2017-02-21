#include "model_class.h"

ModelClass::ModelClass()
{
 vertex_buffer_ = nullptr;
 index_buffer_ = nullptr;
 texture_ = nullptr;
 model_ = nullptr;
}

ModelClass::ModelClass(const ModelClass& other)
{
 
}

ModelClass::~ModelClass()
{
 
}

bool ModelClass::Initialize(ID3D11Device* device, char* model_filename, WCHAR* texture_filename_1, WCHAR* texture_filename_2)
{
 bool result;

 //load model data
 result = Load_model(model_filename);
 if (!result)
  return false;

 //initialize the vertex and index buffer that hold the geometry for the triangle
 result = Initialize_buffers(device);
 if (!result)
  return false;

 //load the texture
 result = Load_texture(device, texture_filename_1, texture_filename_2);
 if (!result)
  return false;
 return true;
}

void ModelClass::Shutdown()
{
 Release_texture();
 Shutdown_buffers();
 Release_model();
}

void ModelClass::Render(ID3D11DeviceContext* device_context)
{
 Render_buffers(device_context);
}

int ModelClass::Get_index_count()
{
 return index_count_;
}

ID3D11ShaderResourceView** ModelClass::Get_texture()
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
 int i;

 vertices = new VERTEX_TYPE[vertex_count_];
 if (!vertices)
  return false;

 indices = new unsigned long[index_count_];
 if (!indices)
  return false;

 for (i = 0; i < vertex_count_; i++)
 {
  //load the vertex array with data
  vertices[i].position = D3DXVECTOR3(model_[i].x, model_[i].y, model_[i].z); //bottom left
  vertices[i].texture = D3DXVECTOR2(model_[i].tu, model_[i].tv);
  vertices[i].normal = D3DXVECTOR3(model_[i].nx, model_[i].ny, model_[i].nz);
  indices[i] = i;
 }

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

bool ModelClass::Load_texture(ID3D11Device* device, WCHAR* filename_1, WCHAR* filename_2)
{
 bool result;

 //create texture object
 texture_ = new TextureClass;
 if (!texture_)
  return false;

 result = texture_->Initialize(device, filename_1, filename_2);
 if (!result)
  return false;

 return true;
}

void ModelClass::Release_texture()
{
 if (texture_)
 {
  texture_->Shutdown();
  delete texture_;
  texture_ = nullptr;
 }
}

bool ModelClass::Load_model(char* file_name)
{
 ifstream fin;
 char input;
 int i;

 fin.open(file_name);

 if (fin.fail())
  return false;

 //read till value of vertex count
 fin.get(input);
 while (input != ':')
  fin.get(input);

 //read vertex count
 fin >> vertex_count_;
 index_count_ = vertex_count_;

 model_ = new MODEL_TYPE[vertex_count_];
 if (!model_)
  return false;

 fin.get(input);
 //'Data:'
 while (input != ':')
  fin.get(input);
 
 //blank lines
 fin.get(input);
 fin.get(input);

 for (i = 0; i < vertex_count_; i++)
 {
  fin >> model_[i].x >> model_[i].y >> model_[i].z;
  fin >> model_[i].tu >> model_[i].tv;
  fin >> model_[i].nx >> model_[i].ny >> model_[i].nz;
 }
 fin.close();
 return true;
}

void ModelClass::Release_model()
{
 if(model_)
 {
  delete [] model_;
  model_ = nullptr;
 }
}




