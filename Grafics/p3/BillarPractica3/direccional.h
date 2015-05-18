#ifndef DIRECCIONAL_H
#define DIRECCIONAL_H

#include "llum.h"

class Direccional : public Llum
{
public:
    Direccional(QGLShaderProgram* program, int id);

private:
    vec3 direccio;
};

#endif // DIRECCIONAL_H
