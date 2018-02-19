#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

int countObjects(ObjectList*);                      // Returns number of objects
void addObject(ObjectList*, Object* );              // Add object to list.
Object* getObjectAt(ObjectList*, int);              // Get object from table index.
void removeObjectAt(ObjectList**, int);             // Remove object at index.
ObjectList* createObjectList();                     // Get created object list.
Object* findObjectByType(ObjectList*, int, int);    // Returns object at found type, and iteration.
void removeObjectPtr(ObjectList* , Object* );

#endif // OBJECT_MANAGER_H
