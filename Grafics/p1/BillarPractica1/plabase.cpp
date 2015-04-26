#include "plabase.h"

/**
 * Creem un pla base amb textures i criem al make.
 * @brief PlaBase::PlaBase
 */
PlaBase::PlaBase():
    Objecte(6),
    _guarda(false),
    transformacions()
{
    setTipus(PLA_BASE);
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

    textures[0] = vec2(0, 0);
    textures[1] = vec2(1, 0);
    textures[2] = vec2(1, 1);
    textures[3] = vec2(0, 0);
    textures[4] = vec2(1, 1);
    textures[5] = vec2(0, 1);

    make();
}

PlaBase::~PlaBase()
{

}

void PlaBase::aplicaTG(mat4 m)
{
    if (_guarda)
    {
        transformacions = m * transformacions;
        inverse(transformacions, inversa);
    }
    Objecte::aplicaTG(m);
}
