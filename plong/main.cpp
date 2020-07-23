//
//  main.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/10/20.
//  Thanks to http://lazyfoo.net tutorials
//

#include <stdio.h>
#include <string>
#include <sstream>

//SDL Specific
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

//Custom Classes
#include "include/Game.h"
#include "include/Ball.h"
#include "include/Paddle.h"
#include "include/Court.h"
#include "include/Texture.h"
#include "include/Text.h"
#include "include/Timer.h"
#include "include/global.h"


int main( int argc, const char * argv[] )
{
    Game pong;
    //Pointers to game window and render to then be assoiated with that window
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRender = NULL;
    
    
    
    //Media loading
    Text titleText;
    Text startText;
    Text fpsText;
    Text pausedText;
    pausedText.setColor({255,255,255,150});
    fpsText.setColor({255,255,255,150});

    
    if( pong.initialize( gWindow, gRender ) )
    {
        //font loading
        //this is really messy, clean up later, make more descriptive
        if ( !titleText.setFont( "include/media/fonts/slkscr.ttf", 80 ) ||
            !startText.setFont( "include/media/fonts/slkscr.ttf", 40 ) ||
            !fpsText.setFont( "include/media/fonts/slkscr.ttf", 20 ) ||
            !pausedText.setFont( "include/media/fonts/slkscr.ttf", 80 ) )
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
        titleText.loadFromText( gRender, "p l o n g" );
        startText.loadFromText( gRender, "press return to start" );
        pausedText.loadFromText( gRender, "paused" );
        
        //In memory text stream, for fps display
        std::stringstream timeText;
        
        //Keep track of game states
        bool started = false; //haven't used yet
        bool reset = false; //testing
        bool paused = true; //maybe there's a better way to do this
        bool quit = false;
        
        //Info for rendering paddles and ball
        Court court;
        Paddle player1( 1 );
        Paddle player2( 2 );
        Ball mBall( 10 );
        mBall.setRandomVector();
        
        //Union that holds event
        SDL_Event event;
        
        int countedFrames = 0;

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
                        case SDLK_RETURN:
                            if( !started )
                            {
                                started = true;
                            }
                    }
                }
                //Control handling
                player1.control( event );
                player2.control( event );
            }
            
            if( !started )
            {
                SDL_SetRenderDrawColor( gRender, 180, 0, 255, 255 );
                SDL_RenderClear( gRender );
                                
                court.render(gRender);
                titleText.render( gRender, ( WINDOW_WIDTH / 2 ) - ( titleText.getWidth() / 2 ), 100 );
                startText.render( gRender, ( WINDOW_WIDTH / 2 ) - ( startText.getWidth() / 2 ), 200 );
            }
            
            //GAME LOOP: UPDATE
            if( started )
            {
                SDL_SetRenderDrawColor( gRender, 180, 0, 255, 255 );
                SDL_RenderClear( gRender );
                
                
                float avgFPS = countedFrames / ( SDL_GetTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }
                
                timeText.str( "" );
                timeText << "FPS " << avgFPS;
                fpsText.loadFromText( gRender, timeText.str().c_str() );
                
                if( paused )
                {
                    pausedText.render( gRender, ( WINDOW_WIDTH / 2 ) - ( pausedText.getWidth() / 2 ), 200 );
                }
                
                if( !paused )
                {

                    player1.move();
                    player2.move();
                    if(!mBall.newMove())
                    {
                        mBall.resetPosition();
                        mBall.setRandomVector();
                        paused = true;
                    }
                    //mBall.move( player1.getPaddleLocation(), player2.getPaddleLocation() );
                }
                
                //GAME LOOP: DRAW
                
                //Clear screen

                fpsText.render( gRender, 10, 10 );
                //Draw paddles & ball
                court.render(gRender);
                //Draw paddles & ball
                player1.render(gRender);
                player2.render(gRender);
                mBall.render( gRender );
                
            }
            
            //Update screen
            SDL_RenderPresent( gRender );
            ++countedFrames;
        }

    }
    
    //closeMedia();
    //could I create a function to take an array of pointers, identify type, close the object, null the pointer?
    startText.free();
    startText.closeFont();
    fpsText.free();
    fpsText.closeFont();
    titleText.free();
    titleText.closeFont();
    
    pong.close( gWindow, gRender );
    return 0;
}
