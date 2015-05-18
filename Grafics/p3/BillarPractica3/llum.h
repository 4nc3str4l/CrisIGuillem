#ifndef LLUM_H
#define LLUM_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

enum TipusLlum
{
    PUNTUAL,
    DIRECCIONAL,
    SPOTLIGHT
};

struct LdLlum
{
    GLuint tipus;

    GLuint posicio;
    GLuint direccio;
    GLuint angle;

    GLuint ld;
    GLuint ls;
    GLuint la;

    GLuint a;
    GLuint b;
    GLuint c;
};

class Llum
{
public:
    Llum(QGLShaderProgram* program, int id_, TipusLlum tipus_);
    ~Llum();

    void toGPU(QGLShaderProgram* program);

    inline void setPosicioLlum(vec4 pos){
        this->posicioLlum = pos;
    }

    inline void setDireccio(vec4 direccio) {
        this->direccio = direccio;
    }

    inline void setAngle(float angle) {
        this->angle = angle;
    }

    inline void setDifusa(vec3 difusa){
        this->difusa = difusa;
    }

    inline void setEspecular(vec3 especular){
        this->especular = especular;
    }

    inline void setAmbient(vec3 ambient){
        this->ambient = ambient;
    }

    inline void setCoeficients(float a, float b, float c) {
        this->coefA = a;
        this->coefB = b;
        this->coefC = c;
    }

private:
    LdLlum gl_ldLlum[SHADING_MAX];
    bool locationsCached[SHADING_MAX];

    int id;
    TipusLlum tipus;

    vec4 posicioLlum;
    vec4 direccio;
    float angle;

    vec3 difusa;
    vec3 especular;
    vec3 ambient;

    float coefA;
    float coefB;
    float coefC;
};

#endif // LLUM_H
