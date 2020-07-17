//
//  Texture.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Texture.h"

Texture::Texture()
{
    loadedTexture = NULL;
    surfaceWidth = 0;
    surfaceHeight = 0;
}

//learn more about destruction, and how it works exactly
Texture::~Texture()
{
    free();
}

void Texture::free()
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

bool Texture::loadFromFile( SDL_Renderer *&render, std::string path )
{
    //free texture just in case anything else was there, useful if new image needs to be loaded into the texture in the game loop
    free(); //
    
    SDL_Surface *loadedSurface = NULL;
    loadedSurface = IMG_Load( path.c_str() );
    
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image (%s) to surface! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        loadedTexture = SDL_CreateTextureFromSurface( render, loadedSurface );
        
        if( loadedTexture == NULL)
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

bool Texture::loadFromText( SDL_Renderer *&render, TTF_Font *&font, std::string textureText, SDL_Color textColor )
{
    //free texture just in case anything else was there, useful if new image needs to be loaded into the texture in the game loop
    free(); //
    
    SDL_Surface *loadedSurface = NULL;
    loadedSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
    
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

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    SDL_SetTextureColorMod( loadedTexture, red, green, blue );
}

void Texture::setAlpha( Uint8 alpha )
{
    SDL_SetTextureAlphaMod( loadedTexture, alpha );
}

void Texture::setBlendMode( SDL_BlendMode blendMode )
{
    SDL_SetTextureBlendMode( loadedTexture, blendMode );
}


void Texture::render( SDL_Renderer *&render, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip )
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
    
    //too much stuff here to explain
    SDL_RenderCopyEx( render, loadedTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
    return surfaceWidth;
}

int Texture::getHeight()
{
    return surfaceHeight;
}
