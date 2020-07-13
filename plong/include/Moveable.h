//
//  Moveable.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/12/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#ifndef Moveable_h
#define Moveable_h

#include <stdio.h>

class Moveable
{
public:
    //Constructor
    Moveable( SDL_Rect &rectangle, int v = 0 );
    
    void setRandomVelocity( void );
    
    void ballMove( SDL_Rect &player1, SDL_Rect &player2 );
    
    void paddleMove( void );
    
    bool checkCollision( SDL_Rect a, SDL_Rect b);
    
    void resetPosition( void );
    
    void controlPlayer1( SDL_Event &event );
    void controlPlayer2( SDL_Event &event );
    
    void render( SDL_Renderer *&render, SDL_Color &color );

private:
    SDL_Rect *mRect;
    
    int rVelX, rVelY;
    
    int rStartX, rStartY;
    
    int velocity;
};

#endif /* Moveable_h */
