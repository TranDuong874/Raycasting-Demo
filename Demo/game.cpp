#include "game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "player.h"

#include <iostream>

using namespace std;


player* square;
Map* mp;
SDL_Renderer* game::renderer = nullptr;

void game::init(const char* title, int posx, int posy, int screen_width, int screen_height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        cout << "Oopsie! Initialization error" << SDL_GetError() << endl;
    else
    {
        window = SDL_CreateWindow(title, posx, posy, screen_width, screen_height, SDL_WINDOW_SHOWN);
        if(window == nullptr) cout << "Oopsie! Window initialization error" << SDL_GetError() << endl;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        if(renderer == nullptr) cout << "Oopsie! Renderer initialization error" << SDL_GetError() << endl;
    }

    square = new player(100,100);
    mp = new Map(16,16,64);

}

void game::userInput()
{
    while(SDL_PollEvent(&event) != 0)
    {
        if(event.type == SDL_QUIT)
        {
            running = false;
        }
        else
        {
            square->input(event);
        }
    }
}

void game::update()
{
    square->update();
}


void game::render()
{
    SDL_RenderClear(renderer);

    mp->Map_Render(renderer);

    square->render();
    square->castRay(mp);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
}



void game::exitSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

