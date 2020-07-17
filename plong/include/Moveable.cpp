//
//  Moveable.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/12/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "Moveable.h"


#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define PI (3.14159265)

//Constructor
//pass rectangle as reference, and then initialize the SDL_Rect pointer to that reference,
//so I can use that to manipulate the rectangle in the game loop
//LEARN NOTE: do I need to set pointer to NULL after I'm done?
Moveable::Moveable( SDL_Rect &rectangle, int v )
{
    mRect = NULL;
    mRect = &rectangle;
    
    //keep track of starting postions, for game starts and resets
    rStartX = mRect->x;
    rStartY = mRect->y;
    
    //for vector math (as needed)
    rVelX = 0;
    rVelY = 0;
    velocity = v; //hypotenuse
}

void Moveable::setRandomVelocity( void )
{
    int angle = rand() % 360;
    
    //sin/cos(x) - x is an angle expressed in radians (1 radian = 180/PI).
    rVelX = velocity * cos(angle*PI/180);
    rVelY = velocity * sin(angle*PI/180);
}

//Only for ball movement
//NEXT STEP: This collision handling sucks, not the right thing at all
//Maybe check for two types of collisions separately to update defelection, one is wall based, the other is paddle based
//FUTURE: Should take into consideration angle of deflection
//FUTURE: Need to update once ball becomes a circle instead of a rectangle
void Moveable::ballMove( SDL_Rect &player1, SDL_Rect &player2 )
{
    //Move the ball based on the current trajectory
    mRect->x += rVelX;
    mRect->y += rVelY;
    
    //now that it has moved, check to see if it will collide with the window edge or paddle
    if( mRect->y <= 0 || mRect->y + mRect->h >= WINDOW_HEIGHT || checkCollision( *mRect, player1) || checkCollision( *mRect, player2) )
    {
        rVelY = rVelY * -1;
    }
    if( mRect->x <= 0 || mRect->x + mRect->w >= WINDOW_WIDTH ||  checkCollision( *mRect, player1) || checkCollision( *mRect, player2) )
    {
        rVelX = rVelX * -1;
    }
}

void Moveable::paddleMove()
{
    //Move the paddle based on handleEvent (rVelY is either positive or negative velocity)
    mRect->y += rVelY;
    
    //now that it has moved, check to see if it will exceed the window size
    if( mRect->y <= 0 || mRect->y + mRect->h >= WINDOW_HEIGHT )
    {
        //if it does, undo the move
        mRect->y -= rVelY;
    }
}

void Moveable::resetPosition()
{
    mRect->x = rStartX;
    mRect->y = rStartY;
}

//Simple box to box collision
bool Moveable::checkCollision( SDL_Rect a, SDL_Rect b )
{
    int aLeft = a.x;
    int aRight = a.x + a.w;
    int aTop = a.y;
    int aBottom = a.y + a.h;
    
    int bLeft = b.x;
    int bRight = b.x + b.w;
    int bTop = b.y;
    int bBottom = b.y + b.h;
    
    if( aBottom <= bTop )
    {
        return false;
    }
    if( aTop >= bBottom )
    {
        return false;
    }
    if( aLeft >= bRight )
    {
        return false;
    }
    if( aRight <= bLeft )
    {
        return false;
    }
    return true;
    
}

//Handles User Controlled Movement for Moveable
//Currently only set for VERTICAL movement of paddle
//NEXT STEP: Really redundant, combine these into one function
//LEARN: about key.repeat and why it defaults to reporting multiple key presses on hold
void Moveable::controlPlayer1( SDL_Event &event )
{
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
    {
        //Update later to refelct, rVelY -= velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_w: rVelY = rVelY - velocity; break;
            case SDLK_s: rVelY = rVelY + velocity; break;
        }
    }
    else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w: rVelY += velocity; break;
            case SDLK_s: rVelY -= velocity; break;
        }
    }
}
void Moveable::controlPlayer2( SDL_Event &event )
{
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
    {
        //Update later to refelct, rVelY -= velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: rVelY = rVelY - velocity; break;
            case SDLK_DOWN: rVelY = rVelY + velocity; break;
        }
    }
    else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: rVelY += velocity; break;
            case SDLK_DOWN: rVelY -= velocity; break;
        }
    }
}

//Renders Moveable
//Pass pointer to render by reference
//Pass color by reference, useful later if I want to update color on a hit. WAIT, does this make sense? Render is called each frame, so color can be passed by value.
//FUTURE: Update to check for texture, in case I want to render that
void Moveable::render( SDL_Renderer *&render, SDL_Color &color )
{
    SDL_SetRenderDrawColor( render, color.r, color.g, color.b, color.a );
    SDL_RenderFillRect( render, mRect );
}

