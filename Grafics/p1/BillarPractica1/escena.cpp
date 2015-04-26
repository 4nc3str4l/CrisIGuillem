#include "escena.h"

/*
 * Permet pasar el tamany de l'ecena com a parametre,
 * */
Escena::Escena(vec3 dimensions)
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin.x = -dimensions.x/2; capsaMinima.pmin.y =-dimensions.y/2; capsaMinima.pmin.z = -dimensions.z/2;
    capsaMinima.pmax.x = dimensions.x/2; capsaMinima.pmax.y = dimensions.y/2; capsaMinima.pmax.z = dimensions.z/2;
    //capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;
    scale = Scale(2/dimensions.x,2/dimensions.y,2/dimensions.z);
}


/*
 * Com tots els objectes formen part de l'escena o son fills de l'escena els destruim iterant sobre ells.
 * I recursivament això eliminarà als fills.
 * */
Escena::~Escena()
{
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        delete (*it);
    }
}

//Afegir un objecte a l'escena.
void Escena::addObjecte(Objecte *obj) {
    objectes.push_back(obj);
}

/* Aquest metode no cal implementar-lo ja que la capça es calcula un cop en el costructor
*/
void Escena::CapsaMinCont3DEscena()
{
}

/*
 * Crida a tots els objectes de l'escena per aplicar-lis una transformació geometrica.
*/
void Escena::aplicaTG(mat4 m) {

    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTG(m);
    }
}


/*
 * Crida a tots els objectes de l'escena per aplicar-lis una transformació geometrica amb origen al punt (0, 0, 0).
*/
void Escena::aplicaTGCentrat(mat4 m) {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTGCentrat(m);
    }
}


/*
 * Crida iterativament al metode draw de tots els objectes de l'escena.
 * */
void Escena::draw() {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->draw();
    }
}

/*
 * Serveix per trobar un tipus d'objecte determinat (util per tenir el tauler en temps d'execució per exemple)
 * */
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

