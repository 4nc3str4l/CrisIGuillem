#ifndef LLUM_H
#define LLUM_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

struct LdLlum
{
    GLuint posicio;

    GLuint ld;
    GLuint ls;
    GLuint la;

    GLuint a;
    GLuint b;
    GLuint c;
};

class Llum
{
    // Evitar que la classe sigui instanciada (cal fer-ho a traves
    // de les subclasses Puntual, Direccional o Spotlight)
protected:
    Llum(QGLShaderProgram* program);

public:
    ~Llum();

    void toGPU(QGLShaderProgram* program);

    inline void setPosicioLlum(vec4 pos){
        this->posicioLlum = pos;
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
    LdLlum gl_ldLlum;

    vec4 posicioLlum;

    vec3 difusa;
    vec3 especular;
    vec3 ambient;

    float coefA;
    float coefB;
    float coefC;
};

#endif // LLUM_H
