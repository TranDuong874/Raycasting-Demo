#include <bits/stdc++.h>
#include <SDL.h>
#include "game.h"

using namespace std;


int main(int arg, char* argv[])
{
    game* g = new game();
    g->init("Love castle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600);

    while(g->running)
    {
        g->userInput();

        g->update();

        g->render();
    }


}
