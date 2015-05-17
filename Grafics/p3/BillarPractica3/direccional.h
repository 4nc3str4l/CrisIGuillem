#ifndef DIRECCIONAL_H
#define DIRECCIONAL_H

#include "llum.h"

class Direccional : public Llum
{
public:
    Direccional(QGLShaderProgram* program);

private:
    vec3 direccio;
};

#endif // DIRECCIONAL_H
