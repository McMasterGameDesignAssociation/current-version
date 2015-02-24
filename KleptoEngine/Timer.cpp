#ifndef TIMER_M
#define TIMER_M

#include "Timer.h"
#include <Windows.h>

Timer::Timer() : 
limit(1000 / 45), previousTiming(GetTickCount64())
{}

Timer::Timer(unsigned int setLimit) :
limit(1000/setLimit), previousTiming(GetTickCount64())
{}

void Timer::setLimit(unsigned int newLimit) { limit = 1000 / newLimit; }

bool Timer::updateTimer(void)
{
	if((GetTickCount64() - previousTiming) >= limit)
	{
		previousTiming = GetTickCount64();
		return true;
	}
	return false;
}

#endif