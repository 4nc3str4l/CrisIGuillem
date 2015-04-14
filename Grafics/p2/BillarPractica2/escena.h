#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>


#include <Common.h>
#include <objecte.h>
#include <taulabillar.h>
#include <plabase.h>

using namespace std;

class Escena
{
public:
    Escena(vec3 dimensions);
    ~Escena();

    void addObjecte(Objecte *obj);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    void CapsaMinCont3DEscena();

    inline Capsa3D getCapsaMinima(){
        return capsaMinima;
    }

    inline mat4 getScale(){
        return scale;
    }

    Objecte* getObjecte(TIPUS_OBJECTE tipus);

private:
    std::vector<Objecte*> objectes;
    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;
    mat4 scale;
};

#endif // ESCENA_H
