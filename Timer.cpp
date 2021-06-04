#include "Timer.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>


std::chrono::steady_clock::time_point startTime;
double duration;
bool isRunning;

Timer::Timer(double durationInSeconds) {
    duration = durationInSeconds;
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
    if (secondsToGo() >= duration)
    {
        stop();
        return true;
    }

    return false;
}

