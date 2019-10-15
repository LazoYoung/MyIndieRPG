#ifndef VECTOR2_INCLUDED
    #define VECTOR2_INCLUDED 1

    typedef struct {
        float x;
        float y;
    } Vector;

    extern Vector addVector(Vector a, Vector b);
    extern Vector subVector(Vector a, Vector b);
#endif