//
//  Timer.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

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
    
    bool isPaused();
    bool isStarted();
    
private:
    //Store ticks
    Uint32 startTicks;
    Uint32 pausedTicks;
    
    bool started;
    bool paused;
};

#endif /* Timer_h */
