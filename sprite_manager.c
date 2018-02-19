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
    spr_unit.w = width * img->xscale;
    spr_unit.h = height * img->yscale;
    SDL_SetTextureColorMod(img->texture, img->color->r, img->color->g, img->color->b);
    SDL_SetTextureAlphaMod(img->texture, img->alpha);
    SDL_SetTextureBlendMode(img->texture, img->blend);
    SDL_RenderCopy(r, img->texture, &spr, &spr_unit);
}

void animateSprite(SDL_Renderer* r, Image* img, uint32_t spd, int x, int y, uint32_t* timer, int* ind, bool draw)
{
    int anim = *ind;
    if(draw)drawSprite(r, img, x, y, anim);
    if( (SDL_GetTicks() - *timer) >= spd )
    {
        (*ind) ++;
        int maxind = (floor((img->surface->w) / img->spritew)-1);
        int maxrows = img->surface->h/img->spriteh;
        if(*ind > (maxind+1)*maxrows) *ind = 0;
        *timer = SDL_GetTicks();
    }
}

void drawSpriteEx(SDL_Renderer* r, Image* img, int x, int y, int ind, double angle, SDL_RendererFlip flip)
{
    int width = img->spritew;
    int height = img->spriteh;
    int maxind = floor((img->surface->w) / width)-1;
    int row = floor( ind/(maxind+1) );
    ind = (row > 0) ? ind-(maxind*row): ind;
    SDL_Rect spr = {ind*width, row*height, ind+width, height};
    SDL_Rect unit_ex = img->unit;
    unit_ex.w = width*img->xscale;
    unit_ex.h = height*img->yscale;
    unit_ex.x = x - (width/2);
    unit_ex.y = y - (height/2);
    SDL_SetTextureColorMod(img->texture, img->color->r, img->color->g, img->color->b);
    SDL_SetTextureAlphaMod(img->texture, img->alpha);
    SDL_SetTextureBlendMode(img->texture, img->blend);
    SDL_RenderCopyEx(r, img->texture, &spr, &unit_ex, angle, NULL, flip);
}
