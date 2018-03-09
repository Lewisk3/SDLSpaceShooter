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
    olist->first = NULL;
    olist->last = NULL;
    return olist;
}

void addObject(ObjectList* base, Object* value)
{
    ObjectNode* current = base->first;
    if(current == NULL)
    {
        printf("[ObjectManager: Info] Adding first object to list: %p \n",value);
        base->length = 1;
        base->objs = malloc(sizeof(ObjectNode));
        base->objs->next = NULL;
        base->objs->obj = value;
        base->first = base->objs;
        base->last = NULL;
        return;
    }
    while(current->obj != NULL && current->next != NULL)
    {
        current = current->next;
    }
    current->next = NULL;
    current->next = malloc(sizeof(ObjectNode));
    current->next->obj = value;
    current->next->next = NULL;
    printf("[ObjectManager: Info] Adding object: %p at %i\n", value, base->length);
    if(base->first == NULL)base->first = current->next;
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
        if(current->obj->AI.type == type)
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

void removeObjectPtr(ObjectList* base, Object* optr)
{
    ObjectNode *prev = NULL;
    ObjectNode *current = base->first;

    while(current->obj != optr && current != NULL)
    {
        prev = current;
        current = current->next;
    }
    printf("[ObjectManager: Info] Removing object %i (Len: %i)\n", current,base->length);
    if(current != NULL)
    {
        if(prev != NULL)
        {
            prev->next = current->next;
        }
        else
        {
            base->objs = current->next;
        }
        if(current == base->first)base->first = current->next;
        if(current == base->last && prev != NULL)base->last = prev;
        free(current);
        base->length--;

    }
    return;
}

void removeObjectAt(ObjectList* base, int index)
{
    printf("[ObjectManager: Info] Removing object at %i \n (Length %i)", index-1,base->length);
    if(index > base->length)
    {
        printf("[ObjectManager: Fatal Error] Invalid node index: %i > %i \n",index-1, base->length);
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

        if(current == base->first && current->next != NULL)base->first = current->next;
        if(current == base->first && current->next == NULL)base->first = NULL;
        if(current == base->last && prev != NULL)base->last = prev;
        // Deallocate us.
        free(current);
        base->length--;
    }
    return;
}
