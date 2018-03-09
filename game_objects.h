#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

Object* createObject(ObjectLogic, Image*, int, int, int, int, bool);
ObjectLogic* createObjectThinker(ObjectTypes, void* , int);
void tickObject(Object* );
void objectImageTransform(Image* ,Object* );
Object* checkObjectCollide(ObjectList* ,Object* );
void drawObject(SDL_Renderer* ,Object* );
double point_direction(int, int, int, int);
void moveOutsideRect(SDL_Rect, SDL_Rect, int* , int* , int);

#endif // GAME_OBJECTS_H
