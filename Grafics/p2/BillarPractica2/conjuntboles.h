#ifndef CONJUNTBOLES_H
#define CONJUNTBOLES_H

#include <vector>
#include "objecte.h"


class ConjuntBoles : public Objecte
{
public:
    ConjuntBoles(QGLShaderProgram* program, Objecte* tauler, std::vector<QOpenGLTexture*> textures);
    ~ConjuntBoles();

    void draw();
    virtual bool intersects(Objecte* obj, vec4 delta);

    Objecte* getBola(int i) {
        return fills[i];
    }
};

#endif // CONJUNTBOLES_H
