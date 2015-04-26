#ifndef BOLA_H
#define BOLA_H

#include "objecte.h"

class Bola : public Objecte
{
public:
    Bola(vec3 color);
    virtual ~Bola();

    void triangle(vec3 a, vec3 b, vec3 c);
    void divide_triangle(vec3 a, vec3 b, vec3 c, int n);
    void generar(int n);

private:
    unsigned int k;
};

#endif // BOLA_H
