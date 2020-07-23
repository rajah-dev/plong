//
//  Game.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/23/20.
//
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "global.h"
#include "Game.h"

Game::Game()
{
    
}

//Initialize SDL and related subsystems
//Seems like a mess, should clean this up later
//Change to SDLLog instead of printf in the future
bool Game::initialize( SDL_Window *&window, SDL_Renderer *&render )
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
        window = SDL_CreateWindow( "p l o n g",
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
void Game::close( SDL_Window *&window, SDL_Renderer *&render )
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
