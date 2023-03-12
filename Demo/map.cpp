#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "game.h"
#include "map.h"

using namespace std;

Map::Map(int Map_Width, int Map_Height/*number of blocs*/, int tileSize)
    {
        this->Map_Width = Map_Width;
        this->Map_Height = Map_Height;
        this->tileSize = tileSize;
        Map_Size = Map_Width*Map_Height;

        mapReader.open("test.txt");

        Map_TileTexture = Map_LoadTexture("tileSet.bmp");
        //Load tiles into a vector
        Map_TileSet = new SDL_Rect[3];
        Map_Tilemap = new int[Map_Size];
        Map_DstTile = new SDL_Rect[Map_Width*Map_Height];

        for(int i = 0; i < 3; i++)
        {
            SDL_Rect tmp = {i*32,0,32,32};
            Map_TileSet[i] = tmp;
        }

        for(int i = 0; i < Map_Size; i++)
        {
            int pos;
            mapReader >> Map_Tilemap[i];
        }

        int k = 0;
        for(int i = 0; i < Map_Height; i++)
        {
            for(int j = 0; j < Map_Width; j++)
            {
                SDL_Rect tmp = {tileSize*j,tileSize*i,tileSize,tileSize};
                Map_DstTile[k]= tmp;
                k++;
            }
        }
    }

void Map::Map_Render(SDL_Renderer* &renderer)
    {
        for(int i = 0; i < Map_Size; i++)
        {

                switch(Map_Tilemap[i])
                {
                case 0:
                    Map_SrcTile = Map_TileSet[0];
                    break;
                case 1:
                    Map_SrcTile = Map_TileSet[1];
                    break;
                case 2:
                    Map_SrcTile = Map_TileSet[2];
                    break;
                }
                SDL_RenderCopy(renderer, Map_TileTexture, &Map_SrcTile, &Map_DstTile[i]);
        }
    }

SDL_Texture* Map::Map_LoadTexture(string path)
    {
        SDL_Surface* loaded_Surface = IMG_Load(path.c_str());
        SDL_Texture* loaded_Texture = SDL_CreateTextureFromSurface(game::renderer, loaded_Surface);
        if(loaded_Surface == nullptr)
        {
            cout << "Unable to load map surface" << endl;
        }
        if(loaded_Texture == nullptr)
            cout << "Unable to load map texture" << endl;
        return loaded_Texture;
    }




