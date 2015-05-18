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

    vec4 color;
};

struct CoefsGPU
{
    GLuint ka;
    GLuint kd;
    GLuint ks;

    GLuint ss;

    GLuint color;
};

class Material
{
public:
    Material(QGLShaderProgram* program, int id, float ka, float kd, float ks, float shinesess, vec4 color = vec4(0, 0, 1, 1));

    void toGPU(QGLShaderProgram* program);

    int getID()
    {
        return id;
    }

private:
    int id;
    Coeficients coefs;
    CoefsGPU coefsGPU;
};

#endif // MATERIAL_H
