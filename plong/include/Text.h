//
//  Text.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/19/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#ifndef Text_h
#define Text_h

class Text {
public:
    Text( );
    ~Text();
    void free();
    void closeFont();
    
    bool setFont( std::string fontPath, int size );
    
    //control display characteristics of font
    void setColor( SDL_Color color );
    
    //takes renderer, font, text string, and color to create surface and then texture
    bool loadFromText( SDL_Renderer *&render, std::string textureText );
    
    //control display characteristics of texture
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
    
private:
    SDL_Texture *loadedTexture;
    TTF_Font *loadedFont;
    SDL_Color textColor;
    //stores dimensions from loaded surface
    //use may change if texture needs to be resized?
    int surfaceWidth;
    int surfaceHeight;
    
};


#endif /* Text_h */
