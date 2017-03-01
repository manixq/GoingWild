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

bool ModelClass::Initialize(ID3D11Device* device, char* model_filename, WCHAR* texture_filename_1, WCHAR* texture_filename_2, WCHAR* texture_filename_3)
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
 result = Load_texture(device, texture_filename_1, texture_filename_2, texture_filename_3);
 if (!result)
  return false;

 Calculate_model_vectors();
 return true;
}

bool ModelClass::Initialize(ID3D11Device* device, char* model_filename, WCHAR* texture_filename_1)
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
 result = Load_texture(device, texture_filename_1);
 if (!result)
  return false;

 Calculate_model_vectors();
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

ID3D11ShaderResourceView** ModelClass::Get_textures()
{
 return texture_->Get_textures();
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
  vertices[i].tangent = D3DXVECTOR3(model_[i].tx, model_[i].ty, model_[i].tz);
  vertices[i].binormal = D3DXVECTOR3(model_[i].bx, model_[i].by, model_[i].bz);
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

void ModelClass::Calculate_model_vectors()
{
 int face_count, i, index;
 TEMP_VERTEX_TYPE vertex1, vertex2, vertex3;
 VECTOR_TYPE tangent, binormal, normal;

 face_count = vertex_count_ / 3;
 index = 0;
 for (i = 0; i < face_count; i++)
 {
  vertex1.x = model_[index].x;
  vertex1.y = model_[index].y;
  vertex1.z = model_[index].z;
  vertex1.tu = model_[index].tu;
  vertex1.tv = model_[index].tv;
  vertex1.nx = model_[index].nx;
  vertex1.ny = model_[index].ny;
  vertex1.nz = model_[index].nz;
  index++;

  vertex2.x = model_[index].x;
  vertex2.y = model_[index].y;
  vertex2.z = model_[index].z;
  vertex2.tu = model_[index].tu;
  vertex2.tv = model_[index].tv;
  vertex2.nx = model_[index].nx;
  vertex2.ny = model_[index].ny;
  vertex2.nz = model_[index].nz;
  index++;

  vertex3.x = model_[index].x;
  vertex3.y = model_[index].y;
  vertex3.z = model_[index].z;
  vertex3.tu = model_[index].tu;
  vertex3.tv = model_[index].tv;
  vertex3.nx = model_[index].nx;
  vertex3.ny = model_[index].ny;
  vertex3.nz = model_[index].nz;
  index++;

  //tangent and binormal of this face
  Calculate_tangent_binormal(vertex1, vertex2, vertex3, tangent, binormal);

  //calculate new normal using tangent and binormal
  Calculate_normal(tangent, binormal, normal);

  model_[index - 1].nx = normal.x;
  model_[index - 1].ny = normal.y;
  model_[index - 1].nz = normal.z;
  model_[index - 1].tx =tangent.x;
  model_[index - 1].ty = tangent.y;
  model_[index - 1].tz = tangent.z;
  model_[index - 1].bx = binormal.x;
  model_[index - 1].by = binormal.y;
  model_[index - 1].bz = binormal.z;

  model_[index - 2].nx = normal.x;
  model_[index - 2].ny = normal.y;
  model_[index - 2].nz = normal.z;
  model_[index - 2].tx = tangent.x;
  model_[index - 2].ty = tangent.y;
  model_[index - 2].tz = tangent.z;
  model_[index - 2].bx = binormal.x;
  model_[index - 2].by = binormal.y;
  model_[index - 2].bz = binormal.z;

  model_[index - 3].nx = normal.x;
  model_[index - 3].ny = normal.y;
  model_[index - 3].nz = normal.z;
  model_[index - 3].tx = tangent.x;
  model_[index - 3].ty = tangent.y;
  model_[index - 3].tz = tangent.z;
  model_[index - 3].bx = binormal.x;
  model_[index - 3].by = binormal.y;
  model_[index - 3].bz = binormal.z;
 }
}

void ModelClass::Calculate_tangent_binormal(TEMP_VERTEX_TYPE vertex1, TEMP_VERTEX_TYPE vertex2, TEMP_VERTEX_TYPE vertex3, VECTOR_TYPE& tangent, VECTOR_TYPE& binormal)
{
 float vector1[3], vector2[3];
 float tu_vector[2], tv_vector[2];
 float den;
 float length;

 vector1[0] = vertex2.x - vertex1.x;
 vector1[1] = vertex2.y - vertex1.y;
 vector1[2] = vertex2.z - vertex1.z;

 vector2[0] = vertex3.x - vertex1.x;
 vector2[1] = vertex3.y - vertex1.y;
 vector2[2] = vertex3.z - vertex1.z;

 tu_vector[0] = vertex2.tu - vertex1.tu;
 tv_vector[0] = vertex2.tv - vertex1.tv;

 tu_vector[1] = vertex3.tu - vertex1.tu;
 tv_vector[1] = vertex3.tv - vertex1.tv;

 //calc denom of the tang/binorm equation
 den = 1.0f / (tu_vector[0] * tv_vector[1] - tu_vector[1] * tv_vector[0]);

 //calculate the cross product and multiply by coefficioent to get the tangent and binormal
 tangent.x = (tv_vector[1] * vector1[0] - tv_vector[0] * vector2[0]) * den;
 tangent.y = (tv_vector[1] * vector1[1] - tv_vector[0] * vector2[1]) * den;
 tangent.z = (tv_vector[1] * vector1[2] - tv_vector[0] * vector2[2]) * den;

 binormal.x = (tu_vector[0] * vector2[0] - tu_vector[1] * vector1[0]) * den;
 binormal.y = (tu_vector[0] * vector2[1] - tu_vector[1] * vector1[1]) * den;
 binormal.z = (tu_vector[0] * vector2[2] - tu_vector[1] * vector1[2]) * den;

 length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
 
 //normalize the normal and then store it
 tangent.x = tangent.x / length;
 tangent.y = tangent.y / length;
 tangent.z = tangent.z / length;

 length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

 //normalize the normal and store it
 binormal.x = binormal.x / length;
 binormal.y = binormal.y / length;
 binormal.z = binormal.z / length;
}

void ModelClass::Calculate_normal(VECTOR_TYPE tangent, VECTOR_TYPE binormal, VECTOR_TYPE& normal)
{
 float length;
 //calc cross product of tangent and binormal
 normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
 normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
 normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

 length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

 //normalize
 normal.x = normal.x / length;
 normal.y = normal.y / length;
 normal.z = normal.z / length;
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

bool ModelClass::Load_texture(ID3D11Device* device, WCHAR* filename_1, WCHAR* filename_2, WCHAR* texture_filename_3)
{
 bool result;

 //create texture object
 texture_ = new TextureClass;
 if (!texture_)
  return false;

 result = texture_->Initialize(device, filename_1, filename_2, texture_filename_3);
 if (!result)
  return false;

 return true;
}

bool ModelClass::Load_texture(ID3D11Device* device, WCHAR* filename_1)
{
 bool result;

 //create texture object
 texture_ = new TextureClass;
 if (!texture_)
  return false;

 result = texture_->Initialize(device, filename_1);
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




