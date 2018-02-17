#include <SDL2/SDL.h>

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

typedef enum
{
    OBJ_BASICENEMY,
} ObjectTypes;

typedef struct
{
    int FORWARD;
    int BACK;
    int LEFT;
    int RIGHT;
    int SHOOT;

} InputButtons;

typedef struct
{
   SDL_Surface* surface;
   SDL_Texture* texture;
   SDL_Rect unit;
   char* name;
} Image;

typedef struct
{
    Image* sprite;
    int width;
    int height;
    float xpos;
    float ypos;
    float speed;
    float friction;
    float xvel;
    float yvel;
    int image_index;
} Player;

typedef struct
{
  Image* img;
  SDL_Rect cam;
} Background;

// Objects

typedef struct
{
    int type;
    int sprite_index;
    int x;
    int y;
    int w;
    int h;

} Object;

typedef struct node_object
{
    Object* obj;
    struct node_object* next;
} ObjectNode;

typedef struct
{
    int length;
    ObjectNode* objs;

} ObjectList;

#endif // STRUCTDATA_H
