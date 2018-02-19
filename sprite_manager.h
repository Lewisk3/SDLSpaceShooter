#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

void drawSprite(SDL_Renderer*, Image*, int, int, int);
void drawSpriteEx(SDL_Renderer*, Image*, int, int, int, double, SDL_RendererFlip);
void animateSprite(SDL_Renderer* , Image* , uint32_t , int , int , uint32_t* , int* , bool);


#endif // SPRITE_MANAGER_H
