#include <stdio.h>
#include <stdlib.h>
#include "structdata.h"
#include "sprite_manager.h"

Object* createObject(int type, int x, int y, int w, int h, int extbytes)
{
    extbytes = (extbytes == 0) ? sizeof(Object) : extbytes;
    if(extbytes < sizeof(Object))
    {
        printf("[Objects] Failed to create object; byte extension not compatible with base object type. Expected ~%i> bytes, got %i. \n", sizeof(Object), extbytes);
        return NULL;
    }
    Object* enemy;
    enemy = malloc(extbytes);
    enemy->sprite_index = 0;
    enemy->angle = 0;
    enemy->xscale = 1;
    enemy->yscale = 1;
    enemy->w = w;
    enemy->h = h;
    enemy->x = x;
    enemy->y = y;
    enemy->w = w;
    enemy->h = h;
    enemy->type = type;

    return enemy;
}

void objectImageTransform(Image* img, Object* obj)
{
    img->unit.x = obj->x + (img->spritew/2);
    img->unit.y = obj->y + (img->spriteh/2);
}

void drawObject(SDL_Renderer* r, Image* img, Object* obj)
{
    drawSpriteEx(r, img, obj->x, obj->y, obj->sprite_index, obj->angle, obj->xscale, obj->yscale, 0);
}
