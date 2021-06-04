#pragma once

#include <xstring>
#include <chrono>

class Timer
{
private:
    std::chrono::steady_clock::time_point startTime;
    double duration;
    bool isRunning;
public:
    Timer(double duration);
    ~Timer();
    void start();
    void stop();
    double secondsToGo();
    std::string secondsToGoString();
    bool hasFinished();
};

