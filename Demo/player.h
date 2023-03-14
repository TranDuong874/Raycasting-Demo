#ifndef player_h
#define player_h

#include "game.h"
#include <math.h>
#include "map.h"
#define PI 3.14159265359
#define PI2 PI/2
#define PI3 3*PI/2
#define D 0.0174533

//Add map scroll to prevent glitch
//Mouse movement
//Add dark fog

//Add render distance
class player
{
public:
    player(double pos_x, double pos_y)
    {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        player_rect.x = pos_x;
        player_rect.y = pos_y;
        player_rect.w = 10;
        player_rect.h = 10;

        step_x = cos(player_angle)*5;
        step_y = sin(player_angle)*5;
    }

    ~player()
    {

    }

    void render()
    {
        SDL_SetRenderDrawColor(game::renderer,255,255,0,255);
        SDL_RenderFillRect(game::renderer,&player_rect);
        SDL_RenderDrawRect(game::renderer, &player_rect);
        SDL_RenderDrawLine(game::renderer,center_x,center_y,center_x+10*step_x,center_y+10*step_y);
    }

    void update()
    {

        player_rect.x = pos_x;
        player_rect.y = pos_y;
        center_x = pos_x + player_rect.w/2;
        center_y = pos_y + player_rect.h/2;

        if(BWD)
        {
            pos_y += step_y;
            pos_x += step_x;
        }

        else if(FWD)
        {

            pos_y -= step_y;
            pos_x -= step_x;
        }

        else if(LEFT)
        {
            player_angle -= 0.01;
            if(player_angle < 0){player_angle += 2*PI;}
            step_x = cos(player_angle)*5;
            step_y = sin(player_angle)*5;
        }
        else if(RIGHT)
        {
            player_angle += 0.01;
            if(player_angle > 2*PI){player_angle -= 2*PI;}
            step_x = cos(player_angle)*5;
            step_y = sin(player_angle)*5;
        }

    }

    void input(SDL_Event event)
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_w:
                BWD = true;
                break;
            case SDLK_s:
                FWD = true;
                break;
            case SDLK_a:
                LEFT = true;
                break;
            case SDLK_d:
                RIGHT = true;
                break;
            }
        }


        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_w:
                BWD = false;
                break;
            case SDLK_s:
                FWD = false;
                break;
            case SDLK_a:
                LEFT = false;
                break;
            case SDLK_d:
                RIGHT = false;
                break;
            }
        }
    }



    void castRay(Map* &mp)
    {
        int map_x, map_y, map_point, dof;//depth of field which is how many lines need to be checked
        float ray_x, ray_y, ray_angle, ray_offset_x, ray_offset_y;
        float v_distance = 1e9, h_distance = 1e9, wall_distance;
        float vx, vy, hx, hy;

        int tile_size = mp->tileSize;
        ray_angle = player_angle-30*D;
        if(ray_angle > 2*PI)
                ray_angle -= 2*PI;
        else if(ray_angle < 0)
                ray_angle += 2*PI;

        for(int r = 0; r < 60; r++)
        {
            //Checking horizontal line
            float aTan = -1/tan(ray_angle);
            dof = 0;
            if(ray_angle > PI)
            {
                ray_y = (((int)center_y/mp->tileSize)*mp->tileSize)-0.0001;
                ray_x = center_x + (center_y-ray_y)*aTan;
                ray_offset_y = -mp->tileSize;
                ray_offset_x = -ray_offset_y*aTan;
            }
            else if(ray_angle < PI)
            {
                ray_y = (((int)center_y/mp->tileSize)*mp->tileSize)+mp->tileSize;
                ray_x = center_x + (center_y-ray_y)*aTan;
                ray_offset_y = mp->tileSize;
                ray_offset_x = -ray_offset_y*aTan;
            }
            else if(ray_angle == 0 || ray_angle == PI)
            {
                ray_x = center_x;
                ray_y = center_y;
                dof = max(mp->Map_Width,mp->Map_Height);
            }

            while(dof < max(mp->Map_Width,mp->Map_Height))
            {
                map_x = (int)ray_x/mp->tileSize;
                map_y = (int)ray_y/mp->tileSize;
                map_point = map_y*mp->Map_Width+map_x;
                hx = ray_x;
                hy = ray_y;
                h_distance = sqrt((ray_x-center_x)*(ray_x-center_x)+(ray_y-center_y)*(ray_y-center_y));
                if(map_point > 0 && map_point < mp->Map_Size && mp->Map_Tilemap[map_point] == 1)
                {

                    break;
                }
                else
                {
                    ray_x += ray_offset_x;
                    ray_y += ray_offset_y;
                    dof++;
                }
            }

            //vertical ray
            dof = 0;
            float nTan = -tan(ray_angle);
            if(ray_angle > PI/2 && ray_angle < 3*PI/2)
            {
                ray_x = (((int)center_x/mp->tileSize)*mp->tileSize)-0.0001;
                ray_y = center_y + (center_x-ray_x)*nTan;
                ray_offset_x = -mp->tileSize;
                ray_offset_y = -ray_offset_x*nTan;
            }
            else if(ray_angle < PI/2 || ray_angle > 3*PI/2)
            {
                ray_x = (((int)center_x/mp->tileSize)*mp->tileSize)+mp->tileSize;
                ray_y = center_y + (center_x-ray_x)*nTan;
                ray_offset_x = mp->tileSize;
                ray_offset_y = -ray_offset_x*nTan;
            }
            else if(ray_angle == 0 || ray_angle == PI)
            {
                ray_x = center_x;
                ray_y = center_y;
                dof = max(mp->Map_Width,mp->Map_Height);
            }

            while(dof < max(mp->Map_Width,mp->Map_Height))
            {
                map_x = (int)ray_x/mp->tileSize;
                map_y = (int)ray_y/mp->tileSize;
                map_point = map_y*mp->Map_Width+map_x;
                vx = ray_x;
                vy = ray_y;
                v_distance = sqrt((ray_x-center_x)*(ray_x-center_x)+(ray_y-center_y)*(ray_y-center_y));
                if(map_point > 0 && map_point < mp->Map_Size && mp->Map_Tilemap[map_point] == 1)
                {

                    break;
                }
                else
                {
                    ray_x += ray_offset_x;
                    ray_y += ray_offset_y;
                    dof++;
                }
            }
            ray_angle += D;
            if(ray_angle > 2*PI)
                ray_angle -= 2*PI;
            else if(ray_angle < 0)
                ray_angle += 2*PI;

            float wall_distance;
            if(v_distance < h_distance)
            {
                SDL_SetRenderDrawColor(game::renderer,0,0,255,255);
                wall_distance = v_distance;
                ray_x = vx;
                ray_y = vy;
            }
            else
            {
                SDL_SetRenderDrawColor(game::renderer,0,0,230,255);
                wall_distance = h_distance;
                ray_x = hx;
                ray_y = hy;
            }

            SDL_RenderDrawLineF(game::renderer,center_x,center_y,ray_x,ray_y);

            //3D Magic
            float cam_angle = player_angle-ray_angle;
            if(cam_angle < 0)
            {
                cam_angle += 2*PI;
            }
            if(cam_angle > 2*PI)
            {
                cam_angle -= 2*PI;
            }
            wall_distance *= cos(cam_angle);
            float wall_height = (160*320)/wall_distance;
            if(wall_height > 320)
                wall_height = 320;

            float wall_offset = 200-wall_height/2;
            for(int k = 0; k < 8; k++)
            {
                SDL_RenderDrawLineF(game::renderer,r*8+512+k,wall_offset,r*8+512+k,wall_offset+wall_height);
            }
        }
        //Check collision

    }



    //To do
    //Add collision
    //Minimap

private:
    float pos_x, pos_y;
    float center_x, center_y;
    float step_x = 5, step_y = 5;
    float player_angle = PI/2;
    SDL_Rect player_rect;
    bool FWD = false, BWD = false, LEFT = false, RIGHT = false;



};
#endif // player_h
