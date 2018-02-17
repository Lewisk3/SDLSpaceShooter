#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "structdata.h"
#include "object_manager.h"
#include "game_objects.h"
#include "sprite_manager.h"


#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 480
#define FRAMERATE 75
#define PHYS_TICKS 60
#define PI 3.14159


enum {
    BT_FORWARD = 0x01,
    BT_BACK    = 0x02,
    BT_LEFT    = 0x04,
    BT_RIGHT   = 0x08,
    BT_FIRE    = 0x10,
    BT_ALTFIRE = 0x20,
};

typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
} States;


// Globals
    static int GameState = GAME_RUNNING;
    static uint8_t active_buttons = 0;
    static Player* plr;
    static InputButtons boundkeys;
    static Background* levelbg;
    static SDL_Window* win;
    static SDL_Renderer* renderer;
    static bool running = true;
    static uint32_t ticks_prev;
    static uint32_t ticks_phys;
    static ObjectList* gameobjects;
    static Image* playerships;
    static Image* enemyships;
    static Image* basicback;
    static Image* lasers;

// Declarations

Image* loadImage(SDL_Renderer*, char*, int, int);
Image* loadSprites(SDL_Renderer*, char*, int, int, int, int);
void freeImage(Image*);
void drawImage(SDL_Renderer*, Image*);
void WindowHandler(SDL_Event* );
void ButtonHandler(InputButtons*, SDL_Event* , uint8_t* );
SDL_Keycode getKey(SDL_Event* );
void Update();
void UpdateObjects();
void DrawObjects();
void Initalize();
void Draw();


int random_range(int, int);

// Game Loop

int main(int argc, char** argv)
{

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Video initialization failed :> %s\n",SDL_GetError());
        return 1; // Return error code.
    }

    // Create SDL window.
    win = SDL_CreateWindow("My SDL Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer,0x00, 0x30, 0x30, 0xFF);


    // Init Objects
    Initalize();
    SDL_Event events;
    ticks_prev = SDL_GetTicks();
    ticks_phys = SDL_GetTicks();
    char* title = malloc(50 * sizeof(char));

    while(running)
    {
        // Handle events;
        if( SDL_GetTicks() > (int)(1000/FRAMERATE)+ticks_prev )
        {
            SDL_SetWindowTitle(win, title);
            while(SDL_PollEvent(&events) != 0)
            {
                WindowHandler(&events);
                ButtonHandler(&boundkeys, &events, &active_buttons);
                if(getKey(&events) == SDLK_p)
                {
                    switch(GameState)
                    {
                        case GAME_PAUSED:  GameState = GAME_RUNNING; break;
                        case GAME_RUNNING: GameState = GAME_PAUSED;  break;
                    }
                }
            }
            ticks_prev = SDL_GetTicks();
        }
         // Update objects/player.
        if(GameState == GAME_RUNNING)
        {
            Update();
        }
        float frametime = (float) (SDL_GetTicks() - ticks_prev);
        int framerate = (int) (1000/frametime);
        sprintf(title, "FPS: %i", framerate);

        Draw();
    }


    freeImage(plr->sprite);
    free(title);
    free(plr);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

// Game functions
void Initalize()
{
    srand(time(NULL));

    // Load Resources / Allocate objects.
    ticks_prev = SDL_GetTicks();

    boundkeys.FORWARD  = SDLK_w;
    boundkeys.BACK     = SDLK_s;
    boundkeys.LEFT     = SDLK_a;
    boundkeys.RIGHT    = SDLK_d;
    boundkeys.SHOOT    = SDL_BUTTON_LEFT;
    boundkeys.ALTSHOOT = SDL_BUTTON_RIGHT;
    playerships = loadSprites(renderer, "Resources/ships.png",0,0,48,45);
    enemyships  = loadSprites(renderer, "Resources/enemyships.png",0,0,48,45);
    basicback   = loadImage(renderer, "Resources/basicback.jpg", 0, 0);
    lasers      = loadSprites(renderer, "Resources/lasers.png",0,0,16,22);

    gameobjects = createObjectList();

    int i;
    for(i = 0; i < 5; i++)
    {
        Object* obj = createObject(OBJ_BASICENEMY,random_range(1, WINDOW_WIDTH-48),random_range(-550, 0),20,40,00);
        obj->sprite_index = 2;
        addObject(gameobjects,obj);
    }


    levelbg = malloc(sizeof(Background));
    levelbg->img = basicback;
    levelbg->cam.x = 0;
    levelbg->cam.y = 850;
    levelbg->cam.w = WINDOW_WIDTH;
    levelbg->cam.h = WINDOW_HEIGHT;
    levelbg->img->unit.w = WINDOW_WIDTH;
    levelbg->img->unit.h = WINDOW_HEIGHT;

    plr = malloc(sizeof(Player));
    plr->sprite = playerships;
    plr->width  = 20;
    plr->height = 40;
    plr->speed  = 6;
    plr->image_index = 2;
    plr->friction = 3.8;
    plr->sprite->unit.w = plr->width;
    plr->sprite->unit.h = plr->height;
    plr->xpos = (WINDOW_WIDTH - plr->width)/2;
    plr->ypos = ((WINDOW_HEIGHT - plr->height)/2);
}


void Update()
{

    if( SDL_GetTicks() > (int)(1000/PHYS_TICKS)+ticks_phys )
    {
        if(active_buttons & BT_FORWARD) plr->yvel -= plr->speed/plr->friction;
        if(active_buttons & BT_BACK) plr->yvel += plr->speed/plr->friction;
        if(active_buttons & BT_LEFT) plr->xvel -= plr->speed/plr->friction;
        if(active_buttons & BT_RIGHT) plr->xvel += plr->speed/plr->friction;
        if(active_buttons & BT_FIRE)
        {
            Bullet* newbullet = createObject(OBJ_SMALLBULLET, plr->xpos, plr->ypos - 18,8,22,sizeof(Bullet));
            newbullet->angle = random_range(-30,30);
            newbullet->speed = 10;
            addObject(gameobjects, newbullet);
        }
                //Move player.
        if(plr->xvel >  plr->speed) plr->xvel = plr->speed;
        if(plr->xvel < -plr->speed) plr->xvel = -plr->speed;
        if(plr->yvel >  plr->speed) plr->yvel = plr->speed;
        if(plr->yvel < -plr->speed) plr->yvel = -plr->speed;
        if(!(active_buttons & BT_FORWARD) && !(active_buttons & BT_BACK)) plr->yvel /= plr->friction;
        if(!(active_buttons & BT_LEFT) && !(active_buttons & BT_RIGHT))   plr->xvel /= plr->friction;
             // Update Player
        plr->xpos += plr->xvel;
        plr->ypos += plr->yvel;
        plr->sprite->unit.x = plr->xpos;
        plr->sprite->unit.y = plr->ypos;
             // Momentum based sprite.
        if(plr->xvel > 0) plr->image_index = 3;
        if(plr->xvel >= plr->speed) plr->image_index = 4;
        if(plr->xvel < 0) plr->image_index = 1;
        if(abs(plr->xvel) <= 0.5) plr->image_index = 2;
        if(plr->xvel <= -plr->speed) plr->image_index = 0;
             //Scroll background
        levelbg->cam.y -= 1;
        if(levelbg->cam.y <= 0)
        {
            levelbg->cam.y = 850;
        }
        UpdateObjects();
        ticks_phys = SDL_GetTicks();
    }
}

void UpdateObjects()
{
    ObjectNode* current = gameobjects->objs;
    int objID = 0;
    while(current != NULL)
    {
        if( current->obj->type == OBJ_DEAD)
        {
           if(current->next != NULL)current = current->next;
           removeObjectAt(gameobjects, objID);
        }
        if( current->obj->type == OBJ_BASICENEMY)
        {
            current->obj->xscale += 0.01;
            current->obj->yscale += 0.01;
            if(current->obj->y > WINDOW_HEIGHT+current->obj->h)
            {
               current->obj->y = random_range(-500,-50);
               current->obj->x = random_range(0, WINDOW_WIDTH-current->obj->w);
            }
            current->obj->y+=2;
        }
        else if( current->obj->type == OBJ_SMALLBULLET)
        {
            Bullet* obullet = current->obj;
            obullet->xvel = cos( (obullet->angle-90) * PI / 180)  * obullet->speed;
            obullet->yvel = sin( (obullet->angle-90) * PI / 180 ) * obullet->speed;

            obullet->x += obullet->xvel;
            obullet->y += obullet->yvel;
            if(obullet->y < 0) obullet->type = OBJ_DEAD;
        }
        objID ++;
        current = current->next;
    }
}

void Draw()
{
     // Draw things
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, levelbg->img->texture, &levelbg->cam, &levelbg->img->unit);
        DrawObjects();
        drawSprite(renderer, playerships, 10,10, 2);
        SDL_RenderPresent(renderer);
}
void DrawObjects()
{
    // Draw player
    drawSpriteEx(renderer, plr->sprite, plr->xpos, plr->ypos, plr->image_index, plr->angle, 1, 1, 0);

    ObjectNode* current = gameobjects->objs;
    while(current != NULL)
    {
        //printf("Object: %p \n", current );
        switch(current->obj->type)
        {
            case OBJ_BASICENEMY:
                drawObject(renderer, enemyships,current->obj);
            break;
            case OBJ_SMALLBULLET:
                drawObject(renderer, lasers,current->obj);
            break;
            default:
            break;
        }
        current = current->next;
    }
}

// Event handlers

void WindowHandler(SDL_Event* ev)
{
    if(ev->type == SDL_WINDOWEVENT)
    {
        switch(ev->window.event)
        {
            case SDL_WINDOWEVENT_CLOSE:
                running = false;
            break;
            default:
            break;
        }
    }
}

void ButtonHandler(InputButtons* input, SDL_Event* ev, uint8_t* abuttons)
{
    if(ev->type == SDL_KEYDOWN)
    {
        int key = ev->key.keysym.sym;
        if(key == input->FORWARD)  *abuttons |= BT_FORWARD;
        if(key == input->BACK)     *abuttons |= BT_BACK;
        if(key == input->LEFT)     *abuttons |= BT_LEFT;
        if(key == input->RIGHT)    *abuttons |= BT_RIGHT;
    }
    else if(ev->type == SDL_KEYUP)
    {
        int key = ev->key.keysym.sym;
        if(key == input->FORWARD)  *abuttons &= ~BT_FORWARD;
        if(key == input->BACK)     *abuttons &= ~BT_BACK;
        if(key == input->LEFT)     *abuttons &= ~BT_LEFT;
        if(key == input->RIGHT)    *abuttons &= ~BT_RIGHT;
    }
    else if(ev->type == SDL_MOUSEBUTTONDOWN)
    {
        if(ev->button.button == input->SHOOT)    *abuttons |= BT_FIRE;
        if(ev->button.button == input->ALTSHOOT) *abuttons |= BT_ALTFIRE;
    }
    else if(ev->type == SDL_MOUSEBUTTONUP)
    {
        if(ev->button.button == input->SHOOT)    *abuttons &= ~BT_FIRE;
        if(ev->button.button == input->ALTSHOOT) *abuttons &= ~BT_ALTFIRE;
    }
}

SDL_Keycode getKey(SDL_Event* e)
{
    if(e->type == SDL_KEYDOWN)
    {
        return e->key.keysym.sym;
    }
}

// Utilities

Image* loadImage(SDL_Renderer* r, char* file, int x, int y)
{
    SDL_Surface* imgsurf = IMG_Load(file);
    if(!imgsurf)
    {
        printf("Failed to load resource: %s \n", file);
        return NULL;
    }
    SDL_Texture* imgtex = SDL_CreateTextureFromSurface(r,imgsurf);
    SDL_Rect imgunit = { x, y, imgsurf->w, imgsurf->h};

    Image* img = NULL;
    img = malloc(sizeof(Image));
    img->name = file;
    img->surface = imgsurf;
    img->texture = imgtex;
    img->unit = imgunit;
    img->spriteh = 32;
    img->spritew = 32;

    printf("Resource loaded: %s; at: Image->%p\n     X->%i, Y->%i\n     W->%i, H->%i\n",file,img,img->unit.x,img->unit.y,img->unit.w,img->unit.h);

    //SDL_DestroyTexture(imgtex);
    //SDL_FreeSurface(imgsurf);

    return img;
}

Image* loadSprites(SDL_Renderer* r, char* file, int x, int y, int w, int h)
{
    Image* img = loadImage(r, file, x, y);
    img->spritew = w;
    img->spriteh = h;
    return img;
}

void drawImage(SDL_Renderer* r, Image* img)
{
    SDL_RenderCopy(r, img->texture, NULL, &img->unit);
}

void freeImage(Image* img)
{
    SDL_DestroyTexture(img->texture);
    SDL_FreeSurface(img->surface);
    free(img);
}

int random_range(int minr, int maxr)
{
    return rand() % ((maxr+1) - minr) + minr;
}
