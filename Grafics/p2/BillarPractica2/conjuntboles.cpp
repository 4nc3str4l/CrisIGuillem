#include "conjuntboles.h"
#include "bola.h"
#include "Common.h"

ConjuntBoles::ConjuntBoles(QGLShaderProgram* program, Objecte* tauler, std::vector<QOpenGLTexture*> textures):
    Objecte()
{
    vec3 colors[] = {
        vec3(189,174,0),
        vec3(229,214,122),
        vec3(13,32,172),
        vec3(224,16,36),
        vec3(42,39,43),
        vec3(12,50,223),
        vec3(158,15,28),
        vec3(137,6,137),
        vec3(147,41,146),
        vec3(240,107,30),
        vec3(129,35,46),
        vec3(129,23,30),
        vec3(42,87,41),
        vec3(42,87,41),
        vec3(196,87,38),

    };

    int k = 1;
    GLfloat z = -100;

    if (tauler)
    {
        capsa = tauler->capsa;
    }
    else
    {
        capsa.pmin = vec3(0,0,0);
        capsa.pmax = vec3(1,1,1);
        capsa.center = vec3(0,0,0);
    }

    // table: 300x440
    // ball: 25x25
    const GLfloat ballTableRelation = 50;
    GLfloat w = (capsa.pmax.x - capsa.pmin.x) / ballTableRelation;
    mat4 scaleMatrix = Scale(w, w, w);
    const int dist = 10;

    for(int i=0; i<15;)
    {
        GLfloat x = -(k*dist/2 - dist/2);
        for (int j = 0; j < k; ++j, ++i) {
            Bola* bola = new Bola(colors[i]/255.0f);
            bola->aplicaTG(Translate(x / ballTableRelation + capsa.center.x,
                                     1 + capsa.center.y,
                                     z / ballTableRelation + capsa.center.z) * scaleMatrix);
            bola->calculCapsa3D();
            bola->toGPU(program, textures[i]);

            boles.push_back(bola);
            x += dist;
        }

        ++k;
        z -= dist;
    }

    capsa = getBola(4)->capsa;

    this->setTipus(CONJUNT_BOLES);
}

ConjuntBoles::~ConjuntBoles()
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        delete *it;
    }
}

void ConjuntBoles::draw()
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        (*it)->draw();
    }
}


void ConjuntBoles::aplicaTG(mat4 m)
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        (*it)->aplicaTG(m);
        (*it)->calculCapsa3D();
    }
}

void ConjuntBoles::aplicaTGPoints(mat4 m)
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        (*it)->aplicaTGPoints(m);
    }
}

bool ConjuntBoles::collides(vec3 pmin, vec3 pmax)
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        if(intersects(pmin, pmax, (*it)->capsa.pmin, (*it)->capsa.pmax))
        {
            return true;
        }
    }

    return false;

}
