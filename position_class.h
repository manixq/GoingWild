#ifndef _POSITION_CLASS_H_
#define _POSITION_CLASS_H_

//========
//Includes
#include <math.h>

//=============
//PositionClass
class PositionClass
{
public:
 PositionClass();
 PositionClass(const PositionClass&);
 ~PositionClass();

 void Set_frame_time(float);
 void Get_rotation(float&);

 void Turn_left(bool);
 void Turn_right(bool);

private:
 float frame_time_;
 float rotation_y_;
 float left_turn_speed_, right_turn_speed_;
};
#endif