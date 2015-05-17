#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>


struct Coeficients
{
    float ka;
    float kd;
    float ks;

    float shineness;
};

struct CoefsGPU
{
    GLuint ka;
    GLuint kd;
    GLuint ks;

    GLuint ss;
};

class Material
{
public:
    Material(QGLShaderProgram* program, float ka, float kd, float ks, float shinesess);

    void toGPU(QGLShaderProgram* program);

private:
    Coeficients coefs;
    CoefsGPU coefsGPU;
};

#endif // MATERIAL_H
