#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_

//=======
//defined
#define WIN32_LEAN_AND_MEAN

//========
//includes
#include <windows.h>

//===============
//my own includes
#include "input_class.h"
#include "graphics_class.h"

//============
//SystemClass
class SystemClass
{
public:
 SystemClass();
 SystemClass(const SystemClass&);
 ~SystemClass();

 bool Initialize();
 void Shutdown();
 void Run();

 LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
 //Winapi func
 bool Frame();
 void Initialize_windows(int&, int&);
 void Shutdown_windows();

 //Winapi var
 LPCWSTR application_name;
 HINSTANCE hinstance;
 HWND hwnd;

 //Engine var
 InputClass* input;
 GraphicsClass* graphics;
};

//===========
//Functions
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//===========
//Globals
static SystemClass* application_handle = 0;

#endif