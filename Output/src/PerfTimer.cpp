// ----------------------------------------------------
// Fast timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"

// L2: TODO 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
// they are simple, one line each!

PerfTimer::PerfTimer()
{
	//...
}

void PerfTimer::Start()
{
	startTime = SDL_GetPerformanceCounter();
	frequency = SDL_GetPerformanceFrequency();
}

double PerfTimer::ReadMs() const
{
	return (double) (SDL_GetPerformanceCounter() - startTime) / frequency * 1000;
}

Uint64 PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - startTime;
}
