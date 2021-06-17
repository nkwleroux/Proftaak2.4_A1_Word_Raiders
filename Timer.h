#pragma once

#include <xstring>
#include <chrono>

class Timer
{
private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    float duration;
    bool isRunning;
    float pauseTimeRemaining;
public:
    Timer(float duration);
    ~Timer();
    void start();
    void stop();
    float timeRemaining();
    std::string timeRemainingToString();
    bool hasFinished();
    void reset();
    void pause();
    void resume();
};

