#ifndef OBJECTE_H
#define OBJECTE_H

#include <QObject>

#include <vector>

#include <Common.h>
#include <cara.h>
#include <readfile.h>

#include <QGLShaderProgram>
#include <QOpenGLTexture>

typedef Common::vec4  color4;
typedef Common::vec4  point4;

using namespace std;

enum TIPUS_OBJECTE
{
    BOLA,
    BOLA_BLANCA,
    CONJUNT_BOLES,
    PLA_BASE,
    TAULER
};

class Objecte : public QObject
{
    Q_OBJECT
protected:
    QString nom; // nom del fitxer on esta el cotxe
    vector<Cara> cares; // cares de l'objecte
    vector<point4> vertexs; // vertexs de l'objecte sense repetits
    vector<point4> vertexColors;

    // Sistema de coordenades d'un objecte: punt origen i eixos de rotació
    GLfloat xorig, yorig, zorig;
    float xRot;
    float yRot;
    float zRot;

    GLfloat tam; // Escala de l'objecte aplicada al fitxer d'entrada


    // Programa de shaders de la GPU
    QGLShaderProgram *program;
    QOpenGLTexture* _texture;
    GLuint buffer; // Buffer de comunicacio amb la GPU

    // Estructures de vertexs i colors per passar a la GPU
    int numPoints;
    int _textureID;
    point4 *points;
    vec3 *normals;
    color4 *colors;
    vec2* textures;

    // Tipus d'objecte
    TIPUS_OBJECTE _tipus;

    // Llista de fills
    std::vector<Objecte*> fills;

public:

  // Capsa mínima contenidora de l'objecte
    Capsa3D capsa;
    vec3 speed;

    Objecte(QObject *parent = 0);
    Objecte(const int npoints, QObject *parent = 0);
    Objecte(const int npoints, QString n);

    virtual ~Objecte();

    // llegeix un model en format OBJ
    virtual void readObj(QString filename);

    // make(): omple l'estructura de points i colors de l'objecte, inicialitza NumVertices
    // Si l'objecte es construeix procedimentalment es sobrecarrega el make
    virtual void make();

    // Pas generic de vertexs i colors a la GPU
    void toGPU(QGLShaderProgram* program, QOpenGLTexture* texture = NULL);
    // Pintat amb la GPU
    virtual void draw();

    // Calcula la capsa 3D contenidora de l'objecte
    Capsa3D calculCapsa3D();

    // Aplica una TG qualsevol a un objecte
    virtual void aplicaTG(mat4 m);
    virtual void aplicaTGPoints(mat4 m);
    // Aplica una TG centrada en el punt central de la capsa de l'objecte a un objecte
    void aplicaTGCentrat(mat4 m, Capsa3D* capsa = NULL);

    virtual bool intersects(Objecte* obj, vec4 delta);

    // Tipus
    TIPUS_OBJECTE getTipus() {
        return _tipus;
    }

    void setTipus(TIPUS_OBJECTE tipus) {
        _tipus = tipus;
    }

    void addChild(Objecte* objecte) {
        fills.push_back(objecte);
    }

    Objecte* getFill(TIPUS_OBJECTE tipus);

private:
    static void construeix_cara ( char **words, int nwords, Objecte*objActual, int vindexUlt)
    {
        Cara face;
        for (int i = 0; i < nwords; i++)
        {
            int vindex;
            int nindex;
            int tindex;

            if ((words[i][0] >= '0') && (words[i][0] <= '9'))
            {
                ReadFile::get_indices(words[i], &vindex, &tindex, &nindex);

    #if DEBUG
                printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
    #endif

                /* store the vertex index */

                if (vindex > 0)       /* indices are from one, not zero */
                    face.idxVertices.push_back(vindex - 1 - vindexUlt);
                else if (vindex < 0)  /* negative indices mean count backwards */
                    face.idxVertices.push_back(objActual->vertexs.size() + vindex);
                else
                {
                    fprintf (stderr, "Zero indices not allowed: '%s'\n", ReadFile::str_orig);
                    exit (-1);
                }
            }
        }

        objActual->cares.push_back(face.setColor(vec4(1.0, 0.0, 0.0, 1.0)));
    }

};

#endif // OBJECTE_H
