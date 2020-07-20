//
//  Ball.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/12/20.
//  Thanks to http://lazyfoo.net tutorials
//

#ifndef Ball_h
#define Ball_h

class Ball
{
public:
    //Constructor
    Ball( int v = 0 );
    
    //maybe create a destructor + free() to turn the pointer to SDL_Rect to NULL?
    
    void setColor( SDL_Color color );
    
    void setRandomVector( void );
    
    void move( SDL_Rect player1, SDL_Rect player2 );
    
    bool newMove( );
    
    bool checkCollision( SDL_Rect a, SDL_Rect b);
    
    void resetPosition( void );
    
    void render( SDL_Renderer *&render );
    
    SDL_Point findIntercept( SDL_Point endPoint, SDL_Point delta, int boundary);

private:
    SDL_Rect ballRect;
    SDL_Color ballColor;
    SDL_Point startPos;
    
    SDL_Point ballCenter;
    SDL_Point delta;
    SDL_Point endVector;
    
    int velocity;
};

#endif /* Ball_h */
