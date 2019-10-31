#ifndef VECTOR2_INCLUDED
#define VECTOR2_INCLUDED

#include "game-struct.h"

extern void add(Vector, Vector, Vector*);
extern void add_m(Vector, float, float, Vector*);
extern void sub(Vector, Vector, Vector*);
extern void scale(Vector, float, Vector*);
extern float distance(Vector, Vector);

#endif