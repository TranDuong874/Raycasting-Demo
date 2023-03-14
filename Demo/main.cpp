#include <bits/stdc++.h>
#include <SDL.h>
#include "game.h"

using namespace std;


int main(int arg, char* argv[])
{
    game* g = new game();
    g->init("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024,512);

    const int FPS = 30;
    const int frameTime = 1000/FPS;
    Uint32 frameStart;
    int frameCnt;

    while(g->running)
    {
        g->userInput();

        g->update();

        g->render();

        frameCnt = SDL_GetTicks()- frameStart;
        if(frameTime > frameCnt)
        {
            SDL_Delay(frameTime - frameCnt);
        }
    }


}
