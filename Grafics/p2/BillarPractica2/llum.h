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

private:
    LdLlum gl_ldLlum;

    vec4 posicioLlum;
    vec3 difusa;
    float coefA;
    float coefB;
    float coefC;
};

#endif // LLUM_H
