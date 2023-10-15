#pragma once
#include <ctime>
#include <chrono>

class Timer
{
private:
    std::chrono::steady_clock::time_point last{};
public:
    Timer();
    long double elapsed();
    void reset();
};