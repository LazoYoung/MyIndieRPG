#ifndef FILE_PHYSIC_INCLUDED
#define FILE_PHYSIC_INCLUDED

#include <stdbool.h>
#include "vector.h"
#include "game.h"
#include "game-struct.h"

extern bool overlaps(AABB, AABB);
extern void updateControl(int, Entity*);
extern void updateEntities();

#endif