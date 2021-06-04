#pragma once

class Timer
{

public:
    Timer(double duration);
    ~Timer();
    void start();
    void stop();
    double secondsToGo();
    bool hasFinished();
};



