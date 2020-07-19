//
//  Timer.cpp
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Thanks to http://lazyfoo.net tutorials
//
//  Simple class to act as a game timer, for score modifiers or time-based challenges.
 
#include <SDL2/SDL.h> //needed for SDL_GetTicks()
#include "Timer.h"

Timer::Timer()
{
    //initialize all properties
    startTicks = 0;
    pausedTicks = 0;
    started = false;
    paused = false;
}

void Timer::start()
{
    //update status
    started = true;
    paused = false;
    
    //when started, store the ticks since SDL was initialized
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::stop()
{
    //reset all properties
    started = false;
    paused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void Timer::pause()
{
    if( started && !paused )
    {
        paused = true;

        //calculate time when paused: get current time and subtract out when timer was started
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause()
{
    if( started && paused )
    {
        paused = false;
        
        //calculate total start time, using time since paused
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    //store time and will return 0 if timer is stopped or not started
    Uint32 time = 0;
    
    if( started )
    {
        if( paused )
        {
            time = pausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - startTicks;
        }
    }
    
    return time;
}

bool Timer::isPaused()
{
    return paused;
}

bool Timer::isStarted()
{
    return started;
}
