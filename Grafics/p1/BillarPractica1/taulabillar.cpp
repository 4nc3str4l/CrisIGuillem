#include "taulabillar.h"

TaulaBillar::TaulaBillar(QString n) : Objecte(NumVerticesF)
{
    nom = n;
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    readObj(n);

    Objecte::make();
    capsa = calculCapsa3D();
}

TaulaBillar::~TaulaBillar()
{

}

