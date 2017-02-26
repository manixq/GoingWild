#include "bitmap_class.h"

BitmapClass::BitmapClass()
{
 vertex_buffer_ = nullptr;
 index_buffer_ = nullptr;
 texture_ = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass&)
{
 
}

BitmapClass::~BitmapClass()
{
 
}

bool BitmapClass::Initialize(ID3D11Device* device, int screen_width, int screen_height, WCHAR* texture_filename, int bitmap_width, int bitmap_height)
{
 bool result;

 screen_width_ = screen_width;
 screen_height_ = screen_height;

 bitmap_width_ = bitmap_width;
 bitmap_height_ = bitmap_height;

 previous_posx_ = -1;
 previous_posy_ = -1;

 result = Initialize_buffers(device);
 if (!result)
  return false;

 result = Load_texture(device, texture_filename);
 if (!result)
  return false;

 return true;
}

void BitmapClass::Shutdown()
{
 Release_texture();
 Shutdown_buffers();
}

bool BitmapClass::Render(ID3D11DeviceContext* device_context, int position_x, int position_y)
{
 bool result;

 result = Update_buffers(device_context, position_x, position_y);
 if (!result)
  return false;

 Render_buffers(device_context);
}

int BitmapClass::Get_index_count()
{
 return index_count_;
}

ID3D11ShaderResourceView* BitmapClass::Get_texture()
{
 return texture_->Get_texture();
}

bool BitmapClass::Initialize_buffers(ID3D11Device* device)
{
 VERTEX_TYPE* vertices;
 unsigned long* indices;
 D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
 D3D11_SUBRESOURCE_DATA vertex_data, index_data;
 HRESULT result;
 int i;

 vertex_count_ = 6;
 index_count_ = vertex_count_;

 vertices = new VERTEX_TYPE[vertex_count_];
 if (!vertices)
  return false;

 indices = new unsigned long[index_count_];
 if (!indices)
  return false;

 memset(vertices, 0, (sizeof(VERTEX_TYPE) * vertex_count_));

 vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
 vertex_buffer_desc.ByteWidth = sizeof(VERTEX_TYPE) * vertex_count_;
 vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
 vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void BitmapClass::Shutdown_buffers()
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
