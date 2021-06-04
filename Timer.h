#pragma once
#include <xstring>

class Timer
{

public:
    Timer(double duration);
    ~Timer();
    void start();
    void stop();
    double secondsToGo();
    std::string secondsToGoString();
    bool hasFinished();
};

