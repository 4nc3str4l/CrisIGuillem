#include "plabase.h"

PlaBase::PlaBase():
    Objecte(6)
{
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    vertexColors.push_back(point4( 0.0, 0.5, 0.0, 1.0));
    vertexColors.push_back(point4( 0.0, 1.0, 0.0, 1.0));

    vertexs.push_back(point4(-10, 0.0, -10, 1.0));
    vertexs.push_back(point4( 10, 0.0, -10, 1.0));
    vertexs.push_back(point4( 10, 0.0, 10,  1.0));
    vertexs.push_back(point4(-10, 0.0, 10,  1.0));

    cares.push_back(Cara(0, 1, 2).setColor(1, 1, 0));
    cares.push_back(Cara(0, 2, 3).setColor(1,0,0));

    make();
}

PlaBase::~PlaBase()
{

}
