#ifndef LLUM_H
#define LLUM_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

struct LdLlum
{
    GLuint posicio;
    GLuint ld;

    GLuint a;
    GLuint b;
    GLuint c;
};

class Llum
{
public:
    Llum(QGLShaderProgram* program);
    ~Llum();

    void toGPU(QGLShaderProgram* program);

    inline void setPosicioLlum(vec4 pos){
        this->posicioLlum = pos;
    }

    inline void setDifusa(vec3 difusa){
        this->difusa = difusa;
    }

    inline void setCoefA(float a){
        this->coefA = a;
    }

    inline void setCoefB(float b){
        this->coefB = b;
    }

    inline void setCoefC(float c){
        this->coefC = c;
    }



private:
    LdLlum gl_ldLlum;

    vec4 posicioLlum;
    vec3 difusa;
    float coefA;
    float coefB;
    float coefC;
};

#endif // LLUM_H
