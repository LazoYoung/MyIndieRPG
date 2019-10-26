#ifndef VECTOR2_INCLUDED
#define VECTOR2_INCLUDED

// TODO Introduce Union as vector storage. Returning a pointer involves headache
typedef float Vector[2];

extern Vector* add(Vector a, Vector b);
extern Vector* add_m(Vector a, float b_x, float b_y);
extern Vector* sub(Vector a, Vector b);
extern Vector* scale(Vector a, float scalar);

#endif