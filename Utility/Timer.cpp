#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}
//elapsed in seconds.
long double Timer::elapsed()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(steady_clock::now() - last).count() / 1000000000.0;
}

void Timer::reset()
{
	last = steady_clock::now();
}