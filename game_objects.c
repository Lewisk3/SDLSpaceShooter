#include <stdio.h>
#include <stdlib.h>
#include "structdata.h"
#include "sprite_manager.h"

Object* createObject(int type, int x, int y, int w, int h, bool solid, int extbytes)
{
    extbytes = (extbytes == 0) ? sizeof(Object) : extbytes;
    if(extbytes < sizeof(Object))
    {
        printf("[Objects] Failed to create object; byte extension not compatible with base object type. Expected ~%i> bytes, got %i. \n", sizeof(Object), extbytes);
        return NULL;
    }
    Object* enemy;
    enemy = malloc(extbytes);
    SDL_Rect pos = {x,y,w,h};
    enemy->sprite_index = 0;
    enemy->solid = solid;
    enemy->angle = 0;
    enemy->xscale = 1;
    enemy->yscale = 1;
    enemy->pos = pos;
    enemy->type = type;

    return enemy;
}

void objectImageTransform(Image* img, Object* obj)
{
    img->unit.x = obj->pos.x + (img->spritew/2);
    img->unit.y = obj->pos.y + (img->spriteh/2);
}

void drawObject(SDL_Renderer* r, Image* img, Object* obj)
{
    drawSpriteEx(r, img, obj->pos.x, obj->pos.y, obj->sprite_index, obj->angle, obj->xscale, obj->yscale, 0);
    SDL_SetRenderDrawColor(r, 0xA0, 0x00, 0xA0, 0xFF);
    //SDL_Rect objbox = {obj->x - (obj->w/2), obj->y - (obj->h/2), obj->w, obj->h};
    //SDL_RenderDrawRect(r, &objbox);
}

Object* checkObjectCollide(ObjectList* base, Object* obj)
{
    ObjectNode* current = base->objs;
    while(current != NULL)
    {
        if(current->obj->type != OBJ_DEAD && current->obj->solid)
        {
             SDL_Rect boxone = obj->pos;
            boxone.x = obj->pos.x - (obj->pos.w/2);
            boxone.y = obj->pos.y - (obj->pos.h/2);
            SDL_Rect boxtwo = current->obj->pos;
            boxtwo.x = current->obj->pos.x - (current->obj->pos.w/2);
            boxtwo.y = current->obj->pos.y - (current->obj->pos.h/2);
            SDL_Rect _results;
            if (SDL_IntersectRect(&boxone, &boxtwo, &_results))
            {
                return current->obj;
            }
        }
        current = current->next;
    }
    return NULL;
}
