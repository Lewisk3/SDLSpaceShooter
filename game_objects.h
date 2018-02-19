#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

Object* createObject(ObjectLogic, Image*, int, int, int, int, bool);
ObjectLogic* createObjectThinker(ObjectTypes, void* , int);
void tickObject(Object* );
void objectImageTransform(Image* ,Object* );
Object* checkObjectCollide(ObjectList* ,Object* );
void drawObject(SDL_Renderer* ,Object* );

#endif // GAME_OBJECTS_H
