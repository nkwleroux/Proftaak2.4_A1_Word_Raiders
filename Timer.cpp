#include "Timer.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>


std::chrono::steady_clock::time_point startTime;
double duration;
bool isRunning;

Timer::Timer(double durationInSeconds) {
    duration = durationInSeconds;
    startTime = std::chrono::steady_clock::now();
}

Timer::~Timer()
{}

void Timer::start()
{
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

void Timer::stop()
{
    isRunning = false;
}

double Timer::secondsToGo()
{
    std::chrono::steady_clock::time_point now;

    if (isRunning)
    {
        now = std::chrono::steady_clock::now();
        double timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0;
        return duration - timePassed;
    }
    else
    {
        return duration;
    }
}

bool Timer::hasFinished() {
    if (secondsToGo() >= duration)
    {
        stop();
        return true;
    }

    return false;
}


