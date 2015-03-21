#include "escena.h"

Escena::Escena(vec3 dimensions)
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin.x = -dimensions.x/2; capsaMinima.pmin.y =-dimensions.y/2; capsaMinima.pmin.z = -dimensions.z/2;
    capsaMinima.pmax.x = dimensions.x/2; capsaMinima.pmax.y = dimensions.y/2; capsaMinima.pmax.z = dimensions.z/2;
    //capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;
    scale = Scale(2/dimensions.x,2/dimensions.y,2/dimensions.z);
}

Escena::~Escena()
{
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        delete (*it);
    }
}

void Escena::addObjecte(Objecte *obj) {
    objectes.push_back(obj);
}


void Escena::CapsaMinCont3DEscena()
{
    // Metode a implementar
}

void Escena::aplicaTG(mat4 m) {

    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTG(m);
    }
}

void Escena::aplicaTGCentrat(mat4 m) {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTGCentrat(m);
    }
}

void Escena::draw() {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->draw();
    }
}

Objecte* Escena::getObjecte(TIPUS_OBJECTE tipus)
{
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        if ((*it)->getTipus() == tipus) {
            return (*it);
        }
    }

    return NULL;
}

