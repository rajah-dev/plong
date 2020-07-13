//
//  main.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/10/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#include <stdio.h>

//SDL Specific
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "include/Moveable.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

//LEARN NOTE: in C++ init() = init(void), in C init() = init(any # of paramters)...learn more about this
bool init( SDL_Window *&window, SDL_Renderer *&render );
void closeGame( SDL_Window *&window, SDL_Renderer *&render );



int main( int argc, const char * argv[] )
{
    //Pointers to game window and render to then be assoiated with that window
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRender = NULL;
        
    if( !init( gWindow, gRender ) )
    {
        printf( "SDL could not initialize.\n" );
    }
    else
    {
        //Keep track of game states
        bool start = false; //haven't used yet
        bool reset = false; //testing
        bool paused = true; //maybe there's a better way to do this
        bool quit = false;
        
        //NOTE: look up SDL_Event
        SDL_Event event;
        
        int paddleWidth = WINDOW_WIDTH / 40;
        int paddleHeight = WINDOW_HEIGHT / 5;
        int ballDiameter = WINDOW_WIDTH / 40;
        
        SDL_Color paddleColor;
        paddleColor.a = 255;
        paddleColor.b = 0;
        paddleColor.g = 0;
        paddleColor.r = 255;

        SDL_Color ballColor;
        ballColor.a = 255;
        ballColor.b = 0;
        ballColor.g = 0;
        ballColor.r = 0;
        
        SDL_Rect player1;
        player1.w = paddleWidth;
        player1.h = paddleHeight;
        player1.x = player1.w * 3;
        player1.y = ( WINDOW_HEIGHT / 2 ) - ( player1.h / 2 );
             
        SDL_Rect player2;
        player2.w = paddleWidth;
        player2.h = paddleHeight;
        player2.x = WINDOW_WIDTH - (player2.w * 3) - player2.w;
        player2.y = ( WINDOW_HEIGHT / 2 ) - ( player2.h / 2 );
        
        SDL_Rect ball;
        ball.w = ballDiameter;
        ball.h = ball.w;
        ball.x = ( WINDOW_WIDTH / 2 ) - ( ball.w / 2 );
        ball.y = ( WINDOW_HEIGHT / 2 ) - ( ball.h / 2 );
        
        Moveable mPlayer1( player1, 5 );
        Moveable mPlayer2( player2, 5 );
        Moveable mBall( ball, 5);
        mBall.setRandomVelocity();
        
        
        //GAME LOOP STARTS
        while( !quit )
        {
            //SDL_PollEvent - Returns 1 if there is a pending event or 0 if there are none available.
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    quit = true;
                }
                //Game state handling
                //NEXT: Clean up
                if( event.type == SDL_KEYDOWN )
                {
                    switch ( event.key.keysym.sym )
                    {
                        case SDLK_SPACE:
                            if( paused )
                            {
                                paused = false;
                            }
                            else
                            {
                                paused = true;
                            }
                            break;
                        case SDLK_r:
                            if( reset )
                            {
                                reset = false;
                            }
                            else
                            {
                                reset = true;
                            }
                            break;
                    }
                }
                //Control handling
                mPlayer1.controlPlayer1( event );
                mPlayer2.controlPlayer2( event );
            }
            
            
            //GAME LOOP: UPDATE
            
            if( !paused )
            {
                mBall.ballMove( player1, player2 );
                mPlayer1.paddleMove();
                mPlayer2.paddleMove();
            }
            
            
            //GAME LOOP: DRAW
            
            //Clear screen
            SDL_SetRenderDrawColor( gRender, 255, 255, 255, 255 );
            SDL_RenderClear( gRender );
            
            //Draw paddles & ball
            mPlayer1.render( gRender, paddleColor );
            mPlayer2.render( gRender, paddleColor );
            mBall.render( gRender, ballColor );
          
            //Update screen
            SDL_RenderPresent( gRender );
            
        }
    }
    
    closeGame( gWindow, gRender );
    return 0;
}

//Initialize SDL and related subsystems
//Seems like a mess, should clean this up later
bool init( SDL_Window *&window, SDL_Renderer *&render )
{
    bool success = true;
    
    //Initialize SDL. SDL_Init should return 0 on success
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        printf( "SDL could not initialize. SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        window = SDL_CreateWindow( "plong",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_RESIZABLE );
        
        if( !window )
        {
            printf( "SDL could not create window. SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Associate renderer to window: creates 2D rendering context for a window
            //ACC = use graphics hardware | VSYNC = no screen tearing, synchronized to refresh rate
            //LEARN: -1 is the value for the index paramter, "the index of the rendering driver to initialize." What's that mean?
            render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

            if( !render )
            {
                printf( "SDL could not associate renderer to window. SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color at the very start
                SDL_SetRenderDrawColor( render, 255, 255, 255, 255 );
                
                
                //Initialize the following subsystems.
                //Currently none of these are actually needed.
                
                //flag needed for PNG loading
                int imgFlags = IMG_INIT_PNG; // IMG_INIT_PNG = 2
                //IMG_Init returns the flags that loaded successfully. Looking for 2 & 2 = 2 = True
                //LEARN: about binary math, see above
                //LEARN MORE IMG_Init
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                
                //initialize SDL_ttf
                //LEARN MORE SDL_ttf
                if( TTF_Init() != 0 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                
                //initialize SDL_mixer
                //LEARN MORE SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}


//Destroy and Close!
//FUTURE: Check for memory leaks!
void closeGame( SDL_Window *&window, SDL_Renderer *&render )
{
    SDL_DestroyRenderer( render );
    render = NULL;

    SDL_DestroyWindow( window );
    window = NULL;
    
    
    //Quit SDL & subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
