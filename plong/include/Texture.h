//
//  Texture.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/16/20.
//  Thanks to http://lazyfoo.net tutorials
//

#ifndef Texture_h
#define Texture_h

#include <stdio.h>

class Texture {
public:
    Texture();
    ~Texture();
    void free();
    
    //takes renderer, and img file source string to create surface and then texture
    bool loadFromFile( SDL_Renderer *&render, std::string path );
    //takes renderer, font, text string, and color to create surface and then texture
    bool loadFromText( SDL_Renderer *&render, TTF_Font *&font, std::string textureText, SDL_Color textColor );

    //control display characteristics of texture
    void setAlpha( Uint8 alpha );
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    void setBlendMode( SDL_BlendMode blendMode );
    
    //pretty much does https://wiki.libsdl.org/SDL_RenderCopyEx, so just look there for the parameters
    void render( SDL_Renderer *&render,
                int x, int y,
                SDL_Rect *clip = NULL,
                double angle = 0.0,
                SDL_Point *center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE );
    
    int getWidth();
    int getHeight();
    
    //may need a function to get SDL_Texture address if I need to manipulate it outside of the class
    
    //how will I want to deal with collision detection?
    
private:
    SDL_Texture *loadedTexture;
    
    //stores dimensions from loaded surface
    //use may change if texture needs to be resized?
    int surfaceWidth;
    int surfaceHeight;
    
    //maybe I should store the render too when constructed, since I use it at least twice?
};

#endif /* Texture_h */
