#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

Object* createObject(int, int, int, int, int, bool, int);
void objectImageTransform(Image*, Object* );
Object* checkObjectCollide(ObjectList*, Object*);

#endif // GAME_OBJECTS_H
