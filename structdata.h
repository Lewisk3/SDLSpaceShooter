#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

typedef enum
{
    OBJ_BASICENEMY,
    OBJ_SMALLBULLET,
    OBJ_DEAD,
    IMG_TIMED,
    OBJ_EXPLOSION,
} ObjectTypes;

typedef struct
{
    int FORWARD;
    int BACK;
    int LEFT;
    int RIGHT;
    int SHOOT;
    int ALTSHOOT;

} InputButtons;

typedef struct
{
   SDL_Surface* surface;
   SDL_Texture* texture;
   SDL_Rect unit;
   int spritew;
   int spriteh;
   uint32_t deltatime;
   char* name;
} Image;

typedef struct
{
    Image* sprite;
    int width;
    int height;
    float xpos;
    float ypos;
    double angle;
    float speed;
    int firerate;
    uint32_t* timers;
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
    uint32_t deltatime;
    double angle;
    double xscale;
    double yscale;
    bool solid;
    SDL_Rect pos;

} Object;

typedef struct
{
    int type;
    int sprite_index;
    uint32_t deltatime;
    double angle;
    double xscale;
    double yscale;
    bool solid;
    SDL_Rect pos;

    int hp;

} Enemy;


typedef struct
{
    int type;
    int sprite_index;
    uint32_t deltatime;
    double angle;
    double xscale;
    double yscale;
    bool solid;
    SDL_Rect pos;

    int dmg;
    int speed;
    int xvel;
    int yvel;

} Bullet;

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
