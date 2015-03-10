#include "escena.h"

Escena::Escena()
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin.x = -1; capsaMinima.pmin.y = -1; capsaMinima.pmin.z = -1;
    capsaMinima.pmax.x = 1; capsaMinima.pmax.y = 1; capsaMinima.pmax.z = 1;
    //capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;
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



