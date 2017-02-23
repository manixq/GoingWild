#include "model_list_class.h"

ModelListClass::ModelListClass()
{
 model_info_list_ = nullptr;
}

ModelListClass::ModelListClass(const ModelListClass&)
{

}

ModelListClass::~ModelListClass()
{
 
}

bool ModelListClass::Initialize(int num_models)
{
 int i;
 float red, green, blue;

 model_count_ = num_models;

 model_info_list_ = new MODEL_INFO_TYPE[model_count_];
 if (!model_info_list_)
  return false;

 srand(static_cast<unsigned int>(time(nullptr)));

 for (i = 0; i < model_count_; i++)
 {
  red = static_cast<float>(rand()) / RAND_MAX;
  green = static_cast<float>(rand()) / RAND_MAX;
  blue = static_cast<float>(rand()) / RAND_MAX;

  model_info_list_[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

  model_info_list_[i].position_x = ((static_cast<float>(rand()) - static_cast<float>(rand()))/ RAND_MAX) * 10.0f;
  model_info_list_[i].position_y = ((static_cast<float>(rand()) - static_cast<float>(rand())) / RAND_MAX) * 10.0f;
  model_info_list_[i].position_z = (((static_cast<float>(rand()) - static_cast<float>(rand())) / RAND_MAX) * 10.0f);
 }
 return true;
}

void ModelListClass::Shutdown()
{
 if(model_info_list_)
 {
  delete[] model_info_list_;
  model_info_list_ = nullptr;
 }
}

int ModelListClass::Get_model_count()
{
 return model_count_;
}


void ModelListClass::Get_data(int index, float& position_x, float& position_y, float& position_z, D3DXVECTOR4& color)
{
 position_x = model_info_list_[index].position_x;
 position_y = model_info_list_[index].position_y;
 position_z = model_info_list_[index].position_z;

 color = model_info_list_[index].color;
}


