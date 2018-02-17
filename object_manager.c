#include <stdio.h>
#include <stdlib.h>
#include "structdata.h"


// List contains, type, sprite_index, hp, x, y, next object pointer.

int countObjects(ObjectList* base)
{
    return base->length;
}

ObjectList* createObjectList()
{
    ObjectList* olist = NULL;
    olist = malloc(sizeof(ObjectList));
    olist->length = 0;
    olist->objs = malloc(sizeof(ObjectNode));
    olist->objs->next = NULL;

    return olist;
}

void addObject(ObjectList* base, Object* value)
{
    ObjectNode* current = base->objs;
    while(current->obj != NULL && current->next != NULL)
    {
        current = current->next;
    }
    current->next = NULL;
    current->next = malloc(sizeof(ObjectNode));
    current->next->obj = value;
    current->next->next = NULL;
    base->length++;
}

Object* getObjectAt(ObjectList* base, int index)
{
    register int i = 0;
    ObjectNode *tmp = NULL;
    ObjectNode *current = base->objs;
    while( (current->obj != NULL && current->next != NULL) && (i < index-1))
    {
        i++;
        current = current->next;
    }
    return current->obj;
}

Object* findObjectByType(ObjectList* base, int type, int index)
{
    register int i = 0;
    ObjectNode* current = base->objs;
    while(current->obj != NULL && current->next != NULL)
    {
        if(current->obj->type == type)
        {
            if(i == index)
            {
                return current->obj;
            }
            else
            {
                i++;
            }
        }
        current = current->next;
    }
    return NULL;
}

void removeObjectAt(ObjectList* base, int index)
{
    if(index > base->length)
    {
        puts("[ObjectManager: Fatal Error] Invalid node index.");
        return;
    }
    ObjectNode *prev = NULL;
    ObjectNode *current = base->objs;

    // Loop through each element until index is reached.
    while(index-- > 0 && current != NULL)
    {
        prev = current;
        current = current->next;
    }
    // If we have an object after this one.
    if(current != NULL)
    {
        // If we have an object before this one, set its next to the one after us.
        if(prev != NULL)
        {
            prev->next = current->next;
        }
        else
        {
            // If we don't simply set us to the next element in our list.
            base->objs = current->next;
        }

        // Deallocate us.
        free(current);
        base->length--;
    }
    return;
}