#ifndef VECTOR2_INCLUDED
#define VECTOR2_INCLUDED

#include "game-struct.h"

extern void add(Vector a, Vector b, Vector*);
extern void add_m(Vector a, float b_x, float b_y, Vector*);
extern void sub(Vector a, Vector b, Vector*);
extern void scale(Vector a, float scalar, Vector*);

#endif