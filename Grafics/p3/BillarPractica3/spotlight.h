#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "llum.h"

class Spotlight : public Llum
{
public:
    Spotlight(QGLShaderProgram* program, int id);

private:
    float angle;
};

#endif // SPOTLIGHT_H
