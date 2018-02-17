#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "structdata.h"


void drawSprite(SDL_Renderer* r, Image* img, int x, int y, int ind)
{
    int width = img->spritew;
    int height = img->spriteh;
    int maxind = floor((img->surface->w) / width)-1;
    int row = floor( ind/(maxind+1) );
    ind = (row > 0) ? ind-(maxind*row): ind;
    SDL_Rect spr = {ind*width, row*height, width, height};
    SDL_Rect spr_unit = img->unit;
    spr_unit.x = x - (width/2);
    spr_unit.y = y - (height/2);
    spr_unit.w = width;
    spr_unit.h = height;
    SDL_RenderCopy(r, img->texture, &spr, &spr_unit);
}

void drawSpriteEx(SDL_Renderer* r, Image* img, int x, int y, int ind, double angle, double xscale, double yscale, SDL_RendererFlip flip)
{
    int width = img->spritew;
    int height = img->spriteh;
    int maxind = floor((img->surface->w) / width)-1;
    int row = floor( ind/(maxind+1) );
    ind = (row > 0) ? ind-(maxind*row): ind;
    SDL_Rect spr = {ind*width, row*height, ind+width, height};
    SDL_Rect unit_ex = img->unit;
    unit_ex.w = width*xscale;
    unit_ex.h = height*yscale;
    unit_ex.x = x - (width/2);
    unit_ex.y = y - (height/2);
    SDL_RenderCopyEx(r, img->texture, &spr, &unit_ex, angle, NULL, flip);
}
