//
//  Game.h
//  plong
//
//  Created by Vinoo Selvarajah on 7/23/20.
//  
//

#ifndef Game_h
#define Game_h

class Game {
public:
    Game( );
    
    bool initialize( SDL_Window *&window, SDL_Renderer *&render );

    void loadMedia();
    
    void gameLoop();
    
    void handleInput();
    
    void update();
    
    void draw();
    
    void close( SDL_Window *&window, SDL_Renderer *&render );

private:
    
    SDL_Window *window;
    SDL_Window *render;
    
    bool quit = false;
    
};

#endif /* Game_h */
