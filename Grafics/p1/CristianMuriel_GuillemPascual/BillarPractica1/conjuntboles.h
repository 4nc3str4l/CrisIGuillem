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
    void aplicaTG(mat4 m);
    void aplicaTGPoints(mat4 m);
    bool collides(vec3 pmin, vec3 pmax);

    Objecte* getBola(int i) {
        return boles[i];
    }

private:
    std::vector<Objecte*> boles;
};

#endif // CONJUNTBOLES_H
