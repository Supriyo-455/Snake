#pragma once

#include <SDL.h>

class Timer
{
public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    uint32_t getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    uint32_t m_StartTicks;

    //The ticks stored when the timer was paused
    uint32_t m_PausedTicks;

    //The timer status
    bool m_Paused;
    bool m_Started;
};

