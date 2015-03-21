#ifndef PLABASE_H
#define PLABASE_H

#include "objecte.h"

class PlaBase : public Objecte
{
public:
    PlaBase();
    ~PlaBase();

    virtual void aplicaTG(mat4 m);

    inline mat4 getTransformacions()
    {
        return transformacions;
    }

    inline mat4 getInversa()
    {
        return inversa;
    }

    inline void guardaTransformacions()
    {
        _guarda = true;
    }

private:
    bool _guarda;
    mat4 transformacions;
    mat4 inversa;
};

#endif // PLABASE_H
