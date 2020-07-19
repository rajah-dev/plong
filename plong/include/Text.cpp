//
//  Text.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/19/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//


#include <string>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Text.h"

Text::Text()
{
    loadedTexture = NULL;
    //should error handle font load
    loadedFont = NULL;
    textColor = { 255, 255, 255, 255 };
    surfaceWidth = 0;
    surfaceHeight = 0;
}

//learn more about destruction, and how it works exactly
Text::~Text()
{
    free();
}

void Text::free()
{
    //if there's a texture loaded, free it
    if( loadedTexture != NULL )
    {
        SDL_DestroyTexture( loadedTexture );
        loadedTexture = NULL;
        //These are set back to 0 cause I'm using free on texture load (loadFromFile/Text)
        surfaceWidth = 0;
        surfaceHeight = 0;
    }
}

void Text::closeFont()
{
    TTF_CloseFont( loadedFont );
    loadedFont = NULL;
}

bool Text::setFont( std::string fontPath, int size )
{
    loadedFont = TTF_OpenFont( fontPath.c_str(), size );
    return loadedFont != NULL;
}

void Text::setColor( SDL_Color color )
{
    textColor = color;
}

bool Text::loadFromText( SDL_Renderer *&render, std::string textureText )
{
    //free texture just in case anything else was there,
    free();
    
    SDL_Surface *loadedSurface = NULL;
    loadedSurface = TTF_RenderText_Solid( loadedFont, textureText.c_str(), textColor );
    
    if( loadedSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        loadedTexture = SDL_CreateTextureFromSurface( render, loadedSurface );
        
        if( loadedTexture == NULL )
        {
            printf( "Unable to render texture from Surface! SDL Error: %s\n", SDL_GetError() );
            //loadedTexture should equal null
        }
        else //everything looks good, so get the dimensions of the surface for later. Could do this earlier?
        {
            surfaceHeight = loadedSurface->h;
            surfaceWidth = loadedSurface->w;
        }
        
        SDL_FreeSurface( loadedSurface );
    }

    return loadedTexture != NULL;
}

void Text::setBlendMode( SDL_BlendMode blendMode )
{
    SDL_SetTextureBlendMode( loadedTexture, blendMode );
}


void Text::render( SDL_Renderer *&render, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip )
{
    //set rendering space and render to screen (dstrect/destination rectangle)
    SDL_Rect renderQuad = { x, y, surfaceWidth, surfaceHeight };
    
    //if a clip was given, use that to adjust the size of the destination rectangle for the render
    if( clip != NULL )
    {
        //don't forget, since clip is a pointer to a rectangle object, need -> to access the member variables/functions
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    SDL_RenderCopyEx( render, loadedTexture, clip, &renderQuad, angle, center, flip);
}

int Text::getWidth()
{
    return surfaceWidth;
}

int Text::getHeight()
{
    return surfaceHeight;
}
