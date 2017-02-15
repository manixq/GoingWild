#ifndef _CAMERA_CLASS_H_
#define  _CAMERA_CLASS_H_

//========
//includes
#include <D3DX10math.h>

//===========
//CameraClass
class CameraClass
{
public:
 CameraClass();
 CameraClass(const CameraClass&);
 ~CameraClass();

 void Set_position(float, float, float);
 void Set_rotation(float, float, float);

 D3DXVECTOR3 Get_position();
 D3DXVECTOR3 Get_rotation();

 void Render();
 void Get_view_matrix(D3DXMATRIX&);

private:
 float position_x_, position_y_, position_z_;
 float rotation_x_, rotation_y_, rotation_z_;
 D3DXMATRIX view_matrix_;
};
#endif