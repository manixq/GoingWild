#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

//============
//InputClass
class InputClass
{
public:
 InputClass();
 InputClass(const InputClass&);
 ~InputClass();

 void Initialize();

 void Key_down(unsigned int);
 void Key_up(unsigned int);

 bool Is_key_down(unsigned int);

private:
 bool keys_[256];
};

#endif