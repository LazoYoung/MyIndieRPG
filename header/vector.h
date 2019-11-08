#ifndef VECTOR2_INCLUDED
#define VECTOR2_INCLUDED

#include "game-struct.h"

void add(Vector, Vector, Vector*);
void add_m(Vector, float, float, Vector*);
void sub(Vector, Vector, Vector*);
void scale(Vector, float, Vector*);
float distance(Vector, Vector);

#endif