#ifndef game_h
#define game_h

#include <SDL.h>


class game
{
public:
    static SDL_Renderer *renderer;


    void init(const char* title, int posx, int posy, int screen_width, int screen_height);

    void userInput();

    void update();

    void render();

    void exitSDL();

    bool running = true;

private:
    SDL_Window *window = nullptr;
    SDL_Event event;

};


#endif
