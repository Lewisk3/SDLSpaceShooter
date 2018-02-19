#include <SDL2/SDL.h>
#include <stdbool.h>

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

typedef enum obj_types
{
    OBJ_BASICENEMY,
    OBJ_BULLET,
    OBJ_EXPLOSION,
    IMG_TIMED,
    OBJ_DEAD,
} ObjectTypes;

typedef struct InputButtons
{
    int FORWARD;
    int BACK;
    int LEFT;
    int RIGHT;
    int SHOOT;
    int ALTSHOOT;

} InputButtons;

typedef struct Colors
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Colors;

typedef struct Image
{
   SDL_Surface* surface;
   SDL_Texture* texture;
   SDL_Rect unit;
   int spritew;
   int spriteh;
   Colors* color;
   uint8_t alpha;
   double xscale;
   double yscale;
   SDL_BlendMode blend;
   int frames;
   uint32_t deltatime;
   char* name;
} Image;

typedef struct Player
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

typedef struct Background
{
  Image* img;
  SDL_Rect cam;
} Background;

// Objects

// A thing list containing, type and tick function ptr.

typedef struct ObjectLogic
{
    ObjectTypes type;
    void* extend;                    // Pointer to extended struct.
    void (*tick)(void*, void*);            // Pointer to tick method.
} ObjectLogic;

typedef struct obj_base
{
    struct ObjectLogic AI;
    int sprite_index;
    Image* sprite;
    uint32_t* timers;
    double angle;
    Colors* color;
    uint8_t alpha;
    double xscale;
    double yscale;
    SDL_BlendMode spriteblend;
    bool solid;
    SDL_Rect pos;

} Object;

typedef struct Enemy
{
    int hp;
    bool hit;

} Enemy;

typedef struct Bullet
{
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

typedef struct ObjectList
{
    int length;
    ObjectNode* objs;

} ObjectList;

#endif // STRUCTDATA_H
