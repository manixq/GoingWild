#include "system_class.h"

SystemClass::SystemClass()
{
 input = nullptr;
 graphics = nullptr;
}

SystemClass::SystemClass(const SystemClass& other_class)
{
}
SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
 int screen_width, screen_height;
 bool result;

 screen_width = 0;
 screen_height = 0;

 //Init winapi
 Initialize_windows(screen_width, screen_height);

 //Keyboard input
 input = new InputClass;
 if (!input) 
  return false;

 //init
 input->Initialize();
 
 //Rendering and graphics
 graphics = new GraphicsClass;
 if (!graphics) 
  return false;

 //init graphics
 result = graphics->Initialize(screen_width, screen_height, hwnd);
 if (!result) 
  return false;

 return true;
}

void SystemClass::Shutdown()
{
 //Release graphics object
 if(graphics)
 {
  graphics->Shutdown();
  delete graphics;
  graphics = nullptr;
 }

 //Release input
 if(input)
 {
  delete input;
  input = nullptr;
 }

 //bye, bye
 Shutdown_windows();
}

void SystemClass::Run()
{
 MSG msg;
 bool done, result;

 //Initialize the message structure (fill with zeros)
 ZeroMemory(&msg, sizeof(MSG));

 //Loop until broken
 done = false;
 while(done)
 {
  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }

  //if window has been closed
  if(msg.message == WM_QUIT)
  {
   done = true;
  }
  else
  {
   //frame proc
   result = Frame();
   if (!result) 
    done = true;
  }
 }
}

bool SystemClass::Frame()
{
 bool result;

 //escape? no.
 if (input->Is_key_down(VK_ESCAPE)) 
  return false;

 //frame processing
 result = graphics->Frame();
 if (!result)
  return false;

 return true;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
 switch(umsg)
 {
  //check if key has been pressed
  case WM_KEYDOWN:
  {
   //if pressed - send to input object to record its state
   input->Key_down(static_cast<unsigned int>(wparam));
   return 0;
  }
  case WM_KEYUP:
  {
   //unset state
   input->Key_up(static_cast<unsigned int>(wparam));
   return 0;
  }
  default: 
   return DefWindowProc(hwnd, umsg, wparam, lparam);
 }
}


void SystemClass::Initialize_windows(int& screen_width, int& screen_height)
{
 WNDCLASSEX wc;
 DEVMODE dm_screen_settings;
 int pos_x, pos_y;

 //external pointer
 application_handle = this;

 //instance of app
 hinstance = GetModuleHandle(nullptr);

 //app name
 application_name = L"Engine";

 //window class
 wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
 wc.lpfnWndProc = WndProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hinstance;
 wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
 wc.hIconSm = wc.hIcon;
 wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
 wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
 wc.lpszMenuName = nullptr;
 wc.lpszClassName = application_name;
 wc.cbSize = sizeof(WNDCLASSEX);

 //register window class
 RegisterClassEx(&wc);
 
 //res
 screen_width = GetSystemMetrics(SM_CXSCREEN);
 screen_height = GetSystemMetrics(SM_CYSCREEN);

 //fullscreen and windowed settings
 if(FULL_SCREEN)
 {
  //fullscreen
  memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
  dm_screen_settings.dmSize = sizeof(dm_screen_settings);
  dm_screen_settings.dmPelsWidth = (unsigned long)screen_width;
  dm_screen_settings.dmPelsHeight = (unsigned long)screen_height;
  dm_screen_settings.dmBitsPerPel = 32;
  dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

  ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN);
  //left top corner
  pos_x = pos_y = 0;
 }
 else
 {
  screen_width = 800;
  screen_height = 600;

  pos_x = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
  pos_y = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
 }

 //create window
 hwnd = CreateWindowEx(
  WS_EX_APPWINDOW,
  application_name,  application_name,
  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
  pos_x, pos_y,
  screen_width, screen_height,
  nullptr, nullptr,
  hinstance, nullptr);

 ShowWindow(hwnd, SW_SHOW);
 SetForegroundWindow(hwnd);
 SetFocus(hwnd);

 ShowCursor(false);
}


void SystemClass::Shutdown_windows()
{
 ShowCursor(true);

 if (FULL_SCREEN)
  ChangeDisplaySettings(nullptr, 0);

 DestroyWindow(hwnd);
 hwnd = nullptr;

 UnregisterClass(application_name, hinstance);
 hinstance = nullptr;

 application_handle = nullptr;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
 switch(umessage)
 {
  case WM_DESTROY:
  {
   PostQuitMessage(0);
   return 0;
  }
  case WM_CLOSE:
  {
   PostQuitMessage(0);
   return 0;
  }
  default:
   return application_handle->MessageHandler(hwnd, umessage, wparam, lparam);
 }
}