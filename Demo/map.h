#ifndef map_h
#define map_h

#include <SDL.h>
#include <SDL_image.h>

#include <bits/stdc++.h>

using namespace std;

struct Map
{
//Variables
    int Map_Width, Map_Height;
    int Map_Size;

    int* Map_Tilemap;
    SDL_Rect* Map_DstTile;

    SDL_Rect* Map_TileSet;
    SDL_Texture* Map_TileTexture;
    SDL_Rect Map_SrcTile;
    int tileSize;

    ifstream mapReader;

//Functions
    Map(int Map_Width, int Map_Height/*number of blocs*/, int tileSize);

    void Map_Render(SDL_Renderer* &renderer);

    SDL_Texture* Map_LoadTexture(string path);

    ~Map(){};


};

#endif // map_h
