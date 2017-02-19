#ifndef _LIGHT_CLASS_H_
#define _LIGHT_CLASS_H_

//========
//Includes
#include <D3DX10math.h>

//==========
//LightClass
class LightClass
{
public:
 LightClass();
 LightClass(const LightClass&);
 ~LightClass();
 
 void Set_ambient_color(float, float, float, float);
 void Set_diffuse_color(float, float, float, float);
 void Set_direction(float, float, float);
 void Set_specular_color(float, float, float, float);
 void Set_specular_power(float);

 D3DXVECTOR4 Get_ambient_color();
 D3DXVECTOR4 Get_diffuse_color();
 D3DXVECTOR3 Get_direction();
 D3DXVECTOR4 Get_specular_color();
 float Get_specular_power();

private:
 D3DXVECTOR4 ambient_color_;
 D3DXVECTOR4 diffuse_color_;
 D3DXVECTOR3 direction_;
 D3DXVECTOR4 specular_color_;
 float specular_power_;
};
#endif