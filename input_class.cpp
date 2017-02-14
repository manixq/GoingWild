#include "input_class.h"

InputClass::InputClass()
{
 
}

InputClass::InputClass(const InputClass&)
{
 
}

InputClass::~InputClass()
{
 
}

void InputClass::Initialize()
{
 int i;
 for (i = 0; i < 256; i++)
  keys_[i] = false;
}

void InputClass::Key_down(unsigned int input)
{
 keys_[input] = true;
}

void InputClass::Key_up(unsigned int input)
{
 keys_[input] = false;
}

bool InputClass::Is_key_down(unsigned int key)
{
 return keys_[key];
}







