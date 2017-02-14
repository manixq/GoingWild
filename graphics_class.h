#ifndef _GRAPHICS_CLASS_H_
#define _GRAPHICS_CLASS_H_

#include <windows.h>

//globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//==============
//GraphicsClass
class GraphicsClass
{
public:
 GraphicsClass();
 GraphicsClass(const GraphicsClass&);
 ~GraphicsClass();

 bool Initialize(int, int, HWND);
 void Shutdown();
 bool Frame();

private:
 bool Render();
};

#endif
