#include <stdio.h>
#include <stdlib.h>
#include "structdata.h"

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
    enemy->x = x;
    enemy->y = y;
    enemy->w = w;
    enemy->h = h;
    enemy->type = type;

    return enemy;
}
