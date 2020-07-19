//
//  Ball.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/12/20.
//  Thanks to http://lazyfoo.net tutorials
//

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "global.h"
#include "Ball.h"


//Constructor
//pass rectangle as reference, and then initialize the SDL_Rect pointer to that reference,
//so I can use that to manipulate the rectangle in the game loop
//LEARN NOTE: do I need to set pointer to NULL after I'm done?
Ball::Ball( int v )
{
    ballRect.w = WINDOW_WIDTH / 40;
    ballRect.h = ballRect.w;
    ballRect.x = (WINDOW_WIDTH / 2) - (ballRect.w / 2);
    ballRect.y = (WINDOW_HEIGHT / 2) - (ballRect.h / 2);

    ballColor = { 255, 255, 255, 255 };
    //keep track of starting postions, for game starts and resets
    rStartX = ballRect.x;
    rStartY = ballRect.y;
    
    //for vector math (as needed)
    rVelX = 0;
    rVelY = 0;
    velocity = v; //hypotenuse
}

void Ball::setColor( SDL_Color color )
{
    ballColor = color;
}

void Ball::setRandomVector( void )
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
void Ball::move( SDL_Rect player1, SDL_Rect player2 )
{
    //Move the ball based on the current trajectory
    ballRect.x += rVelX;
    ballRect.y += rVelY;
    
    //now that it has moved, check to see if it will collide with the window edge or paddle
    if( ballRect.y <= 0 || ballRect.y + ballRect.h >= WINDOW_HEIGHT || checkCollision( ballRect, player1) || checkCollision( ballRect, player2) )
    {
        //undo the previous move since it has collided
        rVelY = rVelY * -1;
        ballRect.y += rVelY;
    }
    if( ballRect.x <= 0 || ballRect.x + ballRect.w >= WINDOW_WIDTH ||  checkCollision( ballRect, player1) || checkCollision( ballRect, player2) )
    {
        rVelX = rVelX * -1;
        ballRect.x += rVelX;
    }
}

void Ball::resetPosition()
{
    ballRect.x = rStartX;
    ballRect.y = rStartY;
}

//Simple box to box collision
bool Ball::checkCollision( SDL_Rect a, SDL_Rect b )
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

//Renders Moveable
//Pass pointer to render by reference
//Pass color by reference, useful later if I want to update color on a hit. WAIT, does this make sense? Render is called each frame, so color can be passed by value.
//FUTURE: Update to check for texture, in case I want to render that
void Ball::render( SDL_Renderer *&render )
{
    SDL_SetRenderDrawColor( render, ballColor.r, ballColor.g, ballColor.b, ballColor.a );
    SDL_RenderFillRect( render, &ballRect );
}

