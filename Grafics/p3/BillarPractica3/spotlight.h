#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "direccional.h"

class Spotlight : public Direccional
{
public:
    Spotlight(QGLShaderProgram* program);

private:
    float angle;
};

#endif // SPOTLIGHT_H
