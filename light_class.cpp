#include "light_class.h"

LightClass::LightClass()
{
 
}

LightClass::LightClass(const LightClass&)
{
 
}

LightClass::~LightClass()
{
 
}

void LightClass::Set_ambient_color(float red, float green, float blue, float alpha)
{
 ambient_color_ = D3DXVECTOR4(red, green, blue, alpha);
}

void LightClass::Set_diffuse_color(float red, float green, float blue, float alpha)
{
 diffuse_color_ = D3DXVECTOR4(red, green, blue, alpha);
}

void LightClass::Set_direction(float x, float y, float z)
{
 direction_ = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 LightClass::Get_ambient_color()
{
 return ambient_color_;
}


D3DXVECTOR4 LightClass::Get_diffuse_color()
{
 return diffuse_color_;
}

D3DXVECTOR3 LightClass::Get_direction()
{
 return direction_;
}


