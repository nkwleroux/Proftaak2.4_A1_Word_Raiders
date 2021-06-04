#include "Timer.h"

#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

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
        return 0;
    }
}

std::string Timer::secondsToGoString()
{
    double rounded = secondsToGo();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << rounded;
    std::string s = stream.str();
    s = "Tijd  " + s;
    return s;
}

bool Timer::hasFinished() {
    if (secondsToGo() <= 0)
    {
        stop();
        return true;
    }

    return false;
}

