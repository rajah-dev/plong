//
//  Paddle.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/18/20.
//  
//

#ifndef Paddle_h
#define Paddle_h

class Paddle
{
public:
    //Constructor
    Paddle( int player );
    
    //Will take copy of a color, could change this to pass by reference, to change colors efficiently in game
    void setColor( SDL_Color color );
    
    void resetPosition( void );
    
    void move( void );
    
    SDL_Rect getPaddleLocation();
    
    void control( SDL_Event &event );
    
    void render( SDL_Renderer *&render );

private:
    SDL_Rect paddleRect; // { x, y, w, h }
    SDL_Color paddleColor; // { r, g, b, a }
    int startPosX, startPosY;
    int velocity;
    int pVelY;
};


#endif /* Paddle_hpp */
