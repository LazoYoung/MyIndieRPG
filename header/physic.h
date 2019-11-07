#ifndef FILE_PHYSIC_INCLUDED
#define FILE_PHYSIC_INCLUDED

#include <stdbool.h>
#include "vector.h"
#include "game.h"
#include "game-struct.h"

bool overlaps(AABB, AABB);
void updateControl(int, Entity*);
void updateEntities();
void updateSwordTrail();

#endif