#include "plabase.h"

PlaBase::PlaBase():
    Objecte(6)
{
    std::cout << "HOLAAA" << std::endl;

    Index = 6;
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    make();
}

PlaBase::~PlaBase()
{

}

void PlaBase::make()
{
    std::cout << "MAKE" << std::endl;

    points[0] = point4(-0.5, -0.5, 0, 1.0);
    points[1] = point4(0.5, -0.5, 0, 1.0);
    points[2] = point4(0.5, 0.5, 0, 1.0);
    points[3] = point4(-0.5, -0.5, 0, 1.0);
    points[4] = point4(0.5, 0.5, 0, 1.0);
    points[5] = point4(-0.5, 0.5, 0, 1.0);

    colors[0] = color4(0, 0.5, 0, 1.0);
    colors[1] = color4(0, 0.5, 0, 1.0);
    colors[2] = color4(0, 1.0, 0, 1.0);
    colors[3] = color4(0, 0.5, 0, 1.0);
    colors[4] = color4(0, 1.0, 0, 1.0);
    colors[5] = color4(0, 1.0, 0, 1.0);
}

