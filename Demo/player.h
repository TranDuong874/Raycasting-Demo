#ifndef player_h
#define player_h

#include "game.h"
#include <math.h>
#include "map.h"
#define PI 3.14159265359
#define PI2 PI/2
#define PI3 3*PI/2
class player
{
public:
    player(double pos_x, double pos_y)
    {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        player_rect.x = pos_x;
        player_rect.y = pos_y;
        player_rect.w = 30;
        player_rect.h = 30;

        step_x = cos(player_angle)*5;
        step_y = sin(player_angle)*5;
    }

    ~player()
    {

    }

    void render()
    {

        SDL_RenderFillRect(game::renderer,&player_rect);
        SDL_RenderDrawRect(game::renderer, &player_rect);
    }

    void update()
    {
        player_rect.x = pos_x;
        player_rect.y = pos_y;
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
            player_angle -= 0.1;
            if(player_angle < 0){player_angle += 2*PI;}
            step_x = cos(player_angle)*5;
            step_y = sin(player_angle)*5;
        }
        else if(RIGHT)
        {
            player_angle += 0.1;
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

    void drawRays(Map* mp)
    {
        int ray, map_x, map_y, map_point, dof;//depth of field which is how many lines need to be checked
        double ray_x, ray_y, ray_angle, ray_offset_x, ray_offset_y;
        ray_angle = player_angle;
        //Check for horizontal lines
        for(int r = 0; r < 1; r++)
        {
            dof = 0;
            double arcTan = -1/tan(ray_angle);
            //look up
            if(ray_angle < PI)
            {
                ray_y = (((int)pos_y*mp->tileSize)/mp->tileSize)+mp->tileSize;
                ray_x = (pos_y-ray_y)*arcTan+pos_x;
                ray_offset_y = mp->tileSize;
                ray_offset_x = -ray_offset_y*arcTan;
            }
            //look down
            if(ray_angle > PI)
            {
                ray_y = (((int)pos_y*mp->tileSize)/mp->tileSize)-0.0001;
                ray_x = (pos_y-ray_y)*arcTan+pos_x;
                ray_offset_y = -mp->tileSize;
                ray_offset_x = -ray_offset_y*arcTan;
            }
            //straight left or right
            if(ray_angle == 0 || ray_angle == PI)
            {
                ray_x = pos_x;
                ray_y = pos_y;
                dof = mp->Map_Height;
            }
            while(dof < mp->Map_Height)
            {
                map_x = (int)ray_x/mp->tileSize;
                map_y = (int)ray_y/mp->tileSize;
                map_point = map_y*mp->Map_Width+map_x;

                if(map_point > 0 && map_point < mp->Map_Width*mp->Map_Height
                   && mp->Map_Tilemap[map_point] == 1)
                {
                    dof = mp->Map_Height;
                }
                else
                {
                    ray_x += ray_offset_x;
                    ray_y += ray_offset_y;
                    dof += 1;
                }
            }
            SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
            SDL_RenderDrawLine(game::renderer,pos_x+(player_rect.w/2),pos_y+(player_rect.h/2),ray_x,ray_y);
        }

        //Check vertical lines
        for(int r = 0; r < 1; r++)
        {
            dof = 0;
            double nTan = -tan(ray_angle);
            //look up
            if(ray_angle < PI2 || ray_angle > PI3)
            {
                ray_x = (((int)pos_x*mp->tileSize)/mp->tileSize)+mp->tileSize;
                ray_y = (pos_x-ray_x)*nTan+pos_y;
                ray_offset_x = mp->tileSize;
                ray_offset_y = -ray_offset_x*nTan;
            }
            //look down
            if(ray_angle > PI2 && ray_angle < PI3)
            {
                ray_x = (((int)pos_x*mp->tileSize)/mp->tileSize)-0.0001;
                ray_y = (pos_x-ray_x)*nTan+pos_y;
                ray_offset_x = -mp->tileSize;
                ray_offset_y = -ray_offset_x*nTan;
            }
            //straight left or right
            if(ray_angle == 0 || ray_angle == PI)
            {
                ray_x = pos_x;
                ray_y = pos_y;
                dof = mp->Map_Height;
            }
            while(dof < mp->Map_Height)
            {
                map_x = (int)ray_x/mp->tileSize;
                map_y = (int)ray_y/mp->tileSize;
                map_point = map_y*mp->Map_Width+map_x;

                if( map_point > 0 && map_point < mp->Map_Width*mp->Map_Height
                   && mp->Map_Tilemap[map_point] == 1)
                {
                    dof = mp->Map_Height;
                }
                else
                {
                    ray_x += ray_offset_x;
                    ray_y += ray_offset_y;
                    dof += 1;
                }
            }
            SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
            SDL_RenderDrawLine(game::renderer,pos_x+(player_rect.w/2),pos_y+(player_rect.h/2),ray_x,ray_y);
        }
    }

private:
    double pos_x, pos_y;
    double step_x = 5, step_y = 5;
    double player_angle = PI/2;

    SDL_Rect player_rect;
    bool FWD = false, BWD = false, LEFT = false, RIGHT = false;
};
#endif // player_h
