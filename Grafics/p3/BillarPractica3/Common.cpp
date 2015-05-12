
#include "Common.h"

/*
 * Magatzem y metodes que s'usen ens diversos llocs per realitzar la practica.
 * */
namespace Common{

    //ens serveix per canviar els modes de vista (o bè en linees o bè en cares)
    bool wireframeView = false;

    void changeViewMode(){
        wireframeView = !wireframeView;
    }

    unsigned int getWireframeView(){
        return wireframeView  ? GL_LINE : GL_FILL;
    }

    //retorna com de gran serà l'escena
    vec3 sceneDimensions()
    {
        return dimensions;
    }

    //Retorna el factor d'escalat en cada dimensió que permet transformar l'objecte de coordenades d'escena a coordenades de pantalla.
    vec3 scaleFactor()
    {
        return scale;
    }
}

