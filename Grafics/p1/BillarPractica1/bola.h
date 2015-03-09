#ifndef BOLA_H
#define BOLA_H

#include "objecte.h"

class Bola : public Objecte
{
public:
    Bola();
    virtual ~Bola();

    void triangle(vec3 a, vec3 b, vec3 c);
    void divide_triangle(vec3 a, vec3 b, vec3 c, int n);
    void tetrahedron(int n);
};

#endif // BOLA_H
