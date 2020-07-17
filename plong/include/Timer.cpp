//
//  Timer.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Timer.h"

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    
    started = false;
    paused = false;
}

void Timer::start()
{
    started = true;
    paused = false;
    
    //when first initialized, stores the ticks when SDL was initialized
    //store the current clock time
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

//reset all members
void Timer::stop()
{
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
        //now that we've paused, get the current init time, minus the start time
        //this is the time at which the clock was paused.
        //calculate time since paused, get current time and subtract start time, to get time at pause
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause()
{
    if( started && paused )
    {
        paused = false;
        //reset starting ticks, get current time and subtract paused time
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    //store time, will return 0 if timer is stopped or not started
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
