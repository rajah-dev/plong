//
//  Timer.h
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Thanks to http://lazyfoo.net tutorials
//
//  Simple class to act as a game timer, for score modifiers or time-based challenges.

#ifndef Timer_h
#define Timer_h

class Timer
{
public:
    Timer();
    
    void start();
    void stop();
    void pause();
    void unpause();
    
    Uint32 getTicks();

    //reports status
    bool isPaused();
    bool isStarted();
    
private:
    //store ticks in milliseconds
    //SDL_GetTicks() - returns a Uint32 for the # of milliseconds since SDL initialized
    Uint32 startTicks;
    Uint32 pausedTicks;
    
    bool started;
    bool paused;
};

#endif /* Timer_h */
