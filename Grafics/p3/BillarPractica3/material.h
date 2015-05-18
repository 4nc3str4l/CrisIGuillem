#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>


struct Coeficients
{
    vec3 ka;
    vec3 kd;
    vec3 ks;

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
    Material(QGLShaderProgram* program, int id, vec3 ka, vec3 kd, vec3 ks, float shinesess, vec4 color = vec4(0, 0, 1, 1));

    void toGPU(QGLShaderProgram* program);

    int getID()
    {
        return id;
    }

private:
    int id;

    Coeficients coefs;
    CoefsGPU coefsGPU[SHADING_MAX];
    bool locationsCached[SHADING_MAX];
};

#endif // MATERIAL_H
