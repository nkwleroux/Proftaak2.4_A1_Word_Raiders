#include "Timer.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Constructor in which we pass the requested duration in seconds
Timer::Timer(float durationInSeconds) {
	// Set start duration
    this->duration = durationInSeconds;
	// Start time is the current time
    this->startTime = std::chrono::steady_clock::now();
	// Initialize with temp variable
    this->endTime = std::chrono::steady_clock::now();
	// Timer isnt running yet
    isRunning = false;
}

// Destructor
Timer::~Timer()
{}

// Method for starting the timer
void Timer::start()
{
    this->startTime = std::chrono::steady_clock::now();
    this->isRunning = true;
}

// Method for stopping the timer
void Timer::stop()
{
    this->endTime = std::chrono::steady_clock::now();
    this->isRunning = false;
}

// Method to request time remaining
float Timer::timeRemaining()
{
    if (isRunning)
    {
    	// Get current time point
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    	// Store timepassed in a variable
        float timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0;

    	// If timepassed is longer then the duration, stop
        if (timePassed>duration)
        {
            stop();
        }

    	// Return  the duration
        return this->duration - timePassed;
    }
    else
    {
    	// Return time passed
        float timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0;
        return this->duration - timePassed;
    }
}

// Method to convert time remaining to string
std::string Timer::timeRemainingToString()
{
	// Using stringstream to convert to ints to string
    float rounded = timeRemaining();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << rounded;
    std::string stringTime = stream.str();
    stringTime = "Tijd  " + stringTime;
    return stringTime;
}

// Returns boolean that answers if the timer has finished
bool Timer::hasFinished() {
	// If there is no more time remaining
    if (timeRemaining() <= 0)
    {
    	// Stop the timer
        stop();
    	// And return true, because timer has finished
        return true;
    }

	// Else return false, because the timer hasnt finished yet
    return false;
}

// Method for resetting the timer
void Timer::reset() {
	// Set starttime
    this->startTime = std::chrono::steady_clock::now();
	// Running = true again
    isRunning = true;
}

// Method to pause the timer
void Timer::pause() {
	// Store timeremaining in a variable
   pauseTimeRemaining = timeRemaining();
	// Stop the timer
   stop();
}

// Method to resume the timer
void Timer::resume() {
	// Resume the timer from previous time
    long long timeSpentFromClock = ((this->duration - pauseTimeRemaining) * 1000.0);
    this->startTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(timeSpentFromClock);
    start();
}