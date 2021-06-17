#include "Timer.h"

#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>


Timer::Timer(float durationInSeconds) {
    this->duration = durationInSeconds;
    this->startTime = std::chrono::steady_clock::now();
    this->endTime = std::chrono::steady_clock::now();
    isRunning = false;
}

Timer::~Timer()
{}

void Timer::start()
{
    this->startTime = std::chrono::steady_clock::now();
    this->isRunning = true;
}

void Timer::stop()
{
    this->endTime = std::chrono::steady_clock::now();
    this->isRunning = false;
}

float Timer::timeRemaining()
{
    if (isRunning)
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        float timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0;
        
        if (timePassed>duration)
        {
            stop();
        }

        return this->duration - timePassed;
    }
    else
    {
        float timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0;
        return this->duration - timePassed;
    }
}

std::string Timer::timeRemainingToString()
{
    float rounded = timeRemaining();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << rounded;
    std::string s = stream.str();
    s = "Tijd  " + s;
    return s;
}

bool Timer::hasFinished() {
    if (timeRemaining() <= 0)
    {
        stop();
        return true;
    }

    return false;
}

