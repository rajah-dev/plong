//
//  Court.cpp
//  plong
//
//  Created by Vinoo Selvarajah on 7/18/20.
//
//

#include <SDL2/SDL.h>
#include "global.h"
#include "Court.h"

Court::Court( void )
{
    courtRect.x = 0;
    courtRect.y = 0;
    courtRect.w = WINDOW_WIDTH;
    courtRect.h = WINDOW_HEIGHT;
    courtColor = { 255, 255, 255, 255 };
}

SDL_Rect Court::getCourtSize()
{
    return courtRect;
}

void Court::setColor( SDL_Color color )
{
    courtColor = color;
}

void Court::render( SDL_Renderer *&render )
{
    SDL_SetRenderDrawColor( render, courtColor.r, courtColor.g, courtColor.b, courtColor.a );
    SDL_RenderDrawRect( render, &courtRect );
    
    for (int y = courtRect.y; y <= courtRect.h; y += 20) {
        SDL_RenderDrawLine( render, WINDOW_WIDTH / 2, y + 5, WINDOW_WIDTH / 2, y + 15);
    }
}
