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
#include "include/Moveable.h"
#include "include/Paddle.h"
#include "include/Court.h"
#include "include/Texture.h"
#include "include/Timer.h"
#include "include/global.h"


//LEARN NOTE: in C++ init() = init(void), in C init() = init(any # of paramters)...learn more about this
bool init( SDL_Window *&window, SDL_Renderer *&render );
void closeGame( SDL_Window *&window, SDL_Renderer *&render );

//Pointless function for now, but think about a class for font loading (and destruction)
//could create the 
TTF_Font *loadFont( std::string path, int fsize )
{
    return TTF_OpenFont( path.c_str(), fsize );
}

int main( int argc, const char * argv[] )
{
    //Pointers to game window and render to then be assoiated with that window
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRender = NULL;
    
    //Media loading
    TTF_Font *largeFont = NULL;
    TTF_Font *mediumFont = NULL;
    TTF_Font *smallFont = NULL;
    Texture titleTexture;
    Texture startTexture;
    Texture controlsTexture;
    Texture scoreTexture;
    Texture fpsTexture;

    
    if( !init( gWindow, gRender ) )
    {
        printf( "SDL could not initialize.\n" );
    }
    else
    {
        //font and texture loading
        //this is really messy, clean up later, make more descriptive
        //turn font load into a class at some point
        largeFont = loadFont( "media/fonts/slkscr.ttf", 60);
        mediumFont = loadFont( "media/fonts/slkscr.ttf", 40);
        smallFont = loadFont( "media/fonts/slkscr.ttf", 20);
        if ( !largeFont || !mediumFont || !smallFont )
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
        //create texture from font, first create surface
        SDL_Color whiteTextColor = { 255, 255, 255};
        SDL_Color greyTextColor = { 200, 200, 200};
        SDL_Color blackTextColor = { 0, 0, 0};
        titleTexture.loadFromText( gRender, largeFont, "p l o n g", whiteTextColor );
        startTexture.loadFromText( gRender, mediumFont, "press return to start", whiteTextColor );
        controlsTexture.loadFromText( gRender, smallFont, "space = pause // r = reset // esc = start menu", greyTextColor );
        scoreTexture.loadFromText( gRender, largeFont, "0", greyTextColor );
        
        
        //Keep track of game states
        bool started = false; //haven't used yet
        bool reset = false; //testing
        bool paused = true; //maybe there's a better way to do this
        bool quit = false;
        
        //Info for rendering paddles and ball
        int paddleWidth = WINDOW_WIDTH / 10;
        int paddleHeight = WINDOW_HEIGHT / 5;
        int ballDiameter = WINDOW_WIDTH / 40;
        
        SDL_Color paddleColor = { 255, 0, 255, 255 };
        SDL_Color ballColor = { 0, 0, 0, 255 };
        
        SDL_Rect player1 = { paddleWidth * 3, ( WINDOW_HEIGHT / 2 ) - ( paddleHeight / 2 ), paddleWidth, paddleHeight };
        SDL_Rect player2 = { WINDOW_WIDTH - (paddleWidth * 3) - paddleWidth, ( WINDOW_HEIGHT / 2 ) - ( paddleHeight / 2 ), paddleWidth, paddleHeight };
        SDL_Rect ball = { ( WINDOW_WIDTH / 2 ) - ( ballDiameter / 2 ), ( WINDOW_HEIGHT / 2 ) - ( ballDiameter / 2 ), ballDiameter, ballDiameter };

        Moveable mPlayer1( player1, 5 );
        Moveable mPlayer2( player2, 5 );
        Moveable mBall( ball, 5);
        mBall.setRandomVelocity();
        
        Court court;
        Paddle test1(2);
        
        //Union that holds event
        //NOTE: look up SDL_Event further
        SDL_Event event;
        
        int countedFrames = 0;
        
        //In memory text stream
        std::stringstream timeText;
        
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
                mPlayer1.controlPlayer1( event );
                mPlayer2.controlPlayer2( event );
                test1.control( event );
            }
            

            //Set text to be rendered
            test1.move();
            
            
            if( !started )
            {
                SDL_SetRenderDrawColor( gRender, 180, 0, 255, 255 );
                SDL_RenderClear( gRender );
                                
                test1.render(gRender);
                court.render(gRender);
                
                titleTexture.render( gRender,
                                        ( WINDOW_WIDTH / 2 ) - ( titleTexture.getWidth() / 2 ), 100 );
                
                startTexture.render( gRender,
                                        ( WINDOW_WIDTH / 2 ) - ( startTexture.getWidth() / 2 ), 200 );
                

                
            }
            
            //GAME LOOP: UPDATE
            if( started )
            {
                float avgFPS = countedFrames / ( SDL_GetTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }
                
                timeText.str( "" );
                timeText << "Average Frames Per Second " << avgFPS;
                fpsTexture.loadFromText( gRender, smallFont, timeText.str().c_str(), blackTextColor );
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
                fpsTexture.render( gRender, 10, 10 );
                //Draw paddles & ball
                test1.render(gRender);
                mPlayer1.render( gRender, paddleColor );
                mPlayer2.render( gRender, paddleColor );
                mBall.render( gRender, ballColor );
                
            }
            
            
            
          
            //Update screen
            SDL_RenderPresent( gRender );
            ++countedFrames;
        }
    }
    
    //closeMedia();
    //could I create a function to take an array of pointers, identify type, close the object, null the pointer?
    titleTexture.free();
    startTexture.free();
    controlsTexture.free();
    scoreTexture.free();
    
    TTF_CloseFont(smallFont);
    smallFont = NULL;
    
    TTF_CloseFont(mediumFont);
    mediumFont = NULL;
    
    TTF_CloseFont(largeFont);
    largeFont = NULL;
    
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
