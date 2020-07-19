//
//  Moveable.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/12/20.
//  Thanks to http://lazyfoo.net tutorials
//

#ifndef Moveable_h
#define Moveable_h

class Moveable
{
public:
    //Constructor
    Moveable( SDL_Rect &rectangle, int v = 0 );
    
    //maybe create a destructor + free() to turn the pointer to SDL_Rect to NULL?
    
    void setRandomVelocity( void );
    
    void ballMove( SDL_Rect &player1, SDL_Rect &player2 );
    
    void paddleMove( void );
    
    bool checkCollision( SDL_Rect a, SDL_Rect b);
    
    void resetPosition( void );
    
    void controlPlayer1( SDL_Event &event );
    void controlPlayer2( SDL_Event &event );
    
    void render( SDL_Renderer *&render, SDL_Color &color );

private:
    SDL_Rect *mRect;
    
    int rVelX, rVelY;
    
    int rStartX, rStartY;
    
    int velocity;
};

#endif /* Moveable_h */
