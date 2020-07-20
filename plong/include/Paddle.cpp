//
//  Paddle.c
//  plong
//
//  Created by Vinoo Selvarajah on 7/18/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//


#include <SDL2/SDL.h>
#include "global.h"
#include "Paddle.h"

Paddle::Paddle( int player )
{
    //initilaize dimensions of the paddle
    //will want to use court dimensions in future
    paddleRect.w = 10;
    paddleRect.x = 0;
    //change this later, but for now will handle player 1 and 2 location
    if( player == 2 )
    {
        paddleRect.x = WINDOW_WIDTH - paddleRect.w;
    }
    paddleRect.h = WINDOW_HEIGHT / 5;
    paddleRect.y = (WINDOW_HEIGHT / 2) - (paddleRect.h / 2);
    
    paddleColor = { 255, 255, 255, 255 };
    
    //keep track of starting postions, for game starts and resets
    startPosX = paddleRect.x;
    startPosY = paddleRect.y;
    
    //defaults for handling motion
    
    velocity = 10;
    pVelY = 0;
}

void Paddle::setColor( SDL_Color color )
{
    paddleColor = color;
}

void Paddle::resetPosition( void )
{
    paddleRect.x = startPosX; //shouldn't have to reset x, since movement will only be vertical
    paddleRect.y = startPosY;
}

SDL_Rect Paddle::getPaddleLocation()
{
    return paddleRect;
}

//Handles User Controlled Movement for Moveable
//Currently only set for VERTICAL movement of paddle
//NEXT STEP: Really redundant, combine these into one function
//LEARN: about key.repeat and why it defaults to reporting multiple key presses on hold
void Paddle::control( SDL_Event &event )
{
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
    {
        //Update later to refelct, rVelY -= velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_w: pVelY = pVelY - velocity; break;
            case SDLK_s: pVelY = pVelY + velocity; break;
        }
    }
    else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w: pVelY += velocity; break;
            case SDLK_s: pVelY -= velocity; break;
        }
    }
    
}

void Paddle::move( void )
{
    //Move the paddle based on handleEvent (rVelY is either positive or negative velocity)
    paddleRect.y += pVelY;
    
    //now that it has moved, check to see if it will exceed the window size
    if( paddleRect.y <= 0 || paddleRect.y + paddleRect.h >= WINDOW_HEIGHT )
    {
        //if it does, undo the move
        paddleRect.y -= pVelY;
    }
}

void Paddle::render( SDL_Renderer *&render )
{
    SDL_SetRenderDrawColor( render, paddleColor.r, paddleColor.g, paddleColor.b, paddleColor.a );
    SDL_RenderFillRect( render, &paddleRect );
}
