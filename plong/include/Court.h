//
//  Court.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/18/20.
//  Copyright © 2020 Vinoo Selvarajah. All rights reserved.
//

#ifndef Court_h
#define Court_h

class Court
{
public:
    //Constructor
    Court( void );
    //Will take copy of a color, could change this to pass by reference, to change colors efficiently in game
    void setColor( SDL_Color color );
    
    SDL_Rect getCourtSize();
    
    void render( SDL_Renderer *&render );

private:
    SDL_Rect courtRect; // { x, y, w, h }
    SDL_Color courtColor; // { r, g, b, a }
};

#endif /* Court_hpp */
