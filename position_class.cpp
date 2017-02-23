#include "position_class.h"

PositionClass::PositionClass()
{
 frame_time_ = 0.0f;
 rotation_y_ = 0.0f;
 left_turn_speed_ = 0.0f;
 right_turn_speed_ = 0.0f;
}

PositionClass::PositionClass(const PositionClass&)
{
 
}

PositionClass::~PositionClass()
{
 
}

void PositionClass::Set_frame_time(float time)
{
 frame_time_ = time;
}

void PositionClass::Get_rotation(float& y)
{
 y = rotation_y_;
}

void PositionClass::Turn_left(bool keydown)
{
 if(keydown)
 {
  left_turn_speed_ +=  0.05f;
  if (left_turn_speed_ > ( 0.5f))
   left_turn_speed_ = 0.5f;
 }
 else
 {
  left_turn_speed_ = 0.0f;
 }

 rotation_y_ -= left_turn_speed_;
 if (rotation_y_ < 0.0f)
  rotation_y_ += 360.0f;
}

void PositionClass::Turn_right(bool keydown)
{
 if (keydown)
 {
  right_turn_speed_ +=0.05f;
  if (right_turn_speed_ >( 0.5f))
   right_turn_speed_ = 0.5f;
 }
 else
 {
  right_turn_speed_ = 0.0f;
 }

 rotation_y_ += right_turn_speed_;
 if (rotation_y_ > 360.0f)
  rotation_y_ -= 360.0f;
}

