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
    GLfloat z = -20;

    // table: 300x440
    // ball: 25x25
    const GLfloat ballTableRelation = 12;
    vec3 scaleFactor = Common::scaleFactor();
    GLfloat w = (tauler->capsa.pmax.x - tauler->capsa.pmin.x) / ballTableRelation;
    mat4 scaleMatrix = Scale(w, w, w);

    for(int i=0; i<15;)
    {
        GLfloat x = -(k - 1);
        for (int j = 0; j < k; ++j, ++i) {
            Bola* bola = new Bola(colors[i]/255);
            bola->toGPU(program, textures[i]);
            bola->aplicaTG(Translate(x / ballTableRelation + tauler->capsa.center.x * scaleFactor.x,
                                     1 / ballTableRelation + tauler->capsa.center.y * scaleFactor.y,
                                     z / ballTableRelation + tauler->capsa.center.z * scaleFactor.z));
            bola->calculCapsa3D();
            bola->aplicaTGCentrat(scaleMatrix);

            cout << tauler->capsa.center.x << " " << tauler->capsa.center.y << " " << tauler->capsa.center.z << endl;

            boles.push_back(bola);
            x += 2;
        }

        ++k;
        z -= 2;
    }

    this->setTipus(CONJUNT_BOLES);

    // Copiar matrius
    capsa.center = tauler->capsa.center;
    capsa.fromCenter = tauler->capsa.fromCenter;
    capsa.toCenter = tauler->capsa.toCenter;
}

ConjuntBoles::~ConjuntBoles()
{

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
    }
}

void ConjuntBoles::aplicaTGPoints(mat4 m)
{
    for (std::vector<Objecte*>::iterator it = boles.begin(); it != boles.end(); ++it)
    {
        (*it)->aplicaTGPoints(m);
    }
}
