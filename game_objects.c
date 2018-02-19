#include <stdio.h>
#include <stdlib.h>
#include "structdata.h"
#include "sprite_manager.h"

// Object Utilities

Object* createObject(ObjectLogic thinker, Image* sprite, int x, int y, int w, int h, bool solid)
{
    // Clone extended struct
    void* extender = malloc( sizeof thinker.extend );
    memcpy(extender,&thinker.extend,sizeof thinker.extend);

    Object* enemy;
    enemy = malloc(sizeof(Object));
    SDL_Rect pos = {x,y,w,h};
    enemy->sprite_index = 0;
    enemy->solid = solid;
    enemy->timers = calloc(10, sizeof(uint32_t));
    enemy->color = malloc(sizeof(Colors));
    enemy->color->r = 0xFF;
    enemy->color->g = 0xFF;
    enemy->color->b = 0xFF;
    enemy->alpha = 0xFF;
    enemy->sprite = sprite;
    enemy->angle = 0;
    enemy->xscale = 1;
    enemy->yscale = 1;
    enemy->pos = pos;
    enemy->spriteblend = SDL_BLENDMODE_BLEND;
    enemy->AI.type   = thinker.type;
    enemy->AI.extend = extender;
    enemy->AI.tick   = thinker.tick;

    return enemy;
}

ObjectLogic* createObjectThinker(ObjectTypes type, void* AI, int ext)
{
    ObjectLogic* othinker;
    othinker = malloc(sizeof(ObjectLogic));
    othinker->type = type;
    othinker->extend = malloc(sizeof(ext));
    othinker->tick = AI;

    return othinker;
}


void objectImageTransform(Object* obj)
{
    Image* img = obj->sprite;
    img->unit.x = obj->pos.x + (img->spritew/2);
    img->unit.y = obj->pos.y + (img->spriteh/2);
    img->alpha = obj->alpha;
    img->color = obj->color;
    img->xscale = obj->xscale;
    img->yscale = obj->yscale;
    img->blend = obj->spriteblend;
}

void drawObject(SDL_Renderer* r, Object* obj)
{
    objectImageTransform(obj);
    drawSpriteEx(r, obj->sprite, obj->pos.x, obj->pos.y, obj->sprite_index, obj->angle, 0);
    //SDL_SetRenderDrawColor(r, 0xA0, 0x00, 0xA0, 0xFF);
    //SDL_Rect objbox = {obj->x - (obj->w/2), obj->y - (obj->h/2), obj->w, obj->h};
    //SDL_RenderDrawRect(r, &objbox);
}

void tickObject(Object* obj)
{
  obj->AI.tick(obj, obj->AI.extend);
}

Object* checkObjectCollide(ObjectList* base, Object* obj)
{
    ObjectNode* current = base->objs->next;
    while(current != NULL)
    {
        if(current->obj->AI.type != OBJ_DEAD && current->obj->solid)
        {
             SDL_Rect boxone = obj->pos;
            boxone.x = obj->pos.x - ((obj->pos.w*obj->xscale)/2);
            boxone.y = obj->pos.y - ((obj->pos.h*obj->xscale)/2);
            boxone.w = obj->pos.w * obj->xscale;
            boxone.h = obj->pos.h * obj->yscale;
            SDL_Rect boxtwo = current->obj->pos;
            boxtwo.x = current->obj->pos.x - ((current->obj->pos.w*obj->xscale)/2);
            boxtwo.y = current->obj->pos.y - ((current->obj->pos.h*obj->xscale)/2);
            boxtwo.w = current->obj->pos.w * current->obj->xscale;
            boxtwo.h = current->obj->pos.h * current->obj->yscale;
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
