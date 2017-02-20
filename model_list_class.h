#ifndef _MODEL_LIST_CLASS_H_
#define _MODEL_LIST_CLASS_H_

//========
//Includes
#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

//==============
//ModelListClass
class ModelListClass
{
public:
 ModelListClass();
 ModelListClass(const ModelListClass&);
 ~ModelListClass();

 bool Initialize(int);
 void Shutdown();

 int Get_model_count();
 void Get_data(int, float&, float&, float&, D3DXVECTOR4&);

private:
 struct MODEL_INFO_TYPE
 {
  D3DXVECTOR4 color;
  float position_x, position_y, position_z;
 };

 int model_count_;
 MODEL_INFO_TYPE* model_info_list_;
};
#endif