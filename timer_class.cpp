#include "timer_class.h"

TimerClass::TimerClass()
{
 
}

TimerClass::TimerClass(const TimerClass&)
{
 
}

TimerClass::~TimerClass()
{
 
}

bool TimerClass::Initialize()
{
 QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency_));
 if (frequency_)
  return false;

 ticks_per_ms_ = static_cast<float>(frequency_ / 1000);
 QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time_));
 return true;
}

void TimerClass::Frame()
{
 INT64 current_time;
 float time_difference;

 QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
 time_difference = static_cast<float>(current_time - start_time_);
 frame_time_ = time_difference / ticks_per_ms_;
 start_time_ = current_time;
}

float TimerClass::Get_time()
{
 return frame_time_;
}
