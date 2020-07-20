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
#include "include/Ball.h"
#include "include/Paddle.h"
#include "include/Court.h"
#include "include/Texture.h"
#include "include/Text.h"
#include "include/Timer.h"
#include "include/global.h"


bool init( SDL_Window *&window, SDL_Renderer *&render );
void closeGame( SDL_Window *&window, SDL_Renderer *&render );

int main( int argc, const char * argv[] )
{
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

    
    if( !init( gWindow, gRender ) )
    {
        printf( "SDL could not initialize.\n" );
    }
    else
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
                
                //initialize SDL_ttf, returns: 0 on success, -1 on any error
                //LEARN MORE: https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html
                if( TTF_Init() == -1 )
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
//use this to close out all standard things from SDL,
//will need to create a separate function to close out non-standard bits
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
