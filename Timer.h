#pragma once

#include <xstring>
#include <chrono>

class Timer
{
private:
	// C++ includes support for two types of time manipulation:
    // The chrono library, a flexible collection of types that track time with varying degrees of precision(e.g.std::chrono::time_point).
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

	// Other variables we need to keep track of
    float duration;
    bool isRunning;
    float pauseTimeRemaining;

public:
	// Constructor
    Timer(float duration);
	// Destructor
    ~Timer();
	// Method for starting the timer
    void start();
	// Method for stopping the timer
    void stop();
	// Method to request time remaining
    float timeRemaining();
	// Method to convert time remaining to string
    std::string timeRemainingToString();
	// Boolean if the timer has finished
    bool hasFinished();
	// Method to reset the timer
    void reset();
	// Method to pause the timer
    void pause();
	// Method to resume the timer
    void resume();
};

