#include "material.h"

Material::Material(QGLShaderProgram* program, float ka, float kd, float ks, float shinesess)
{
    coefsGPU.ka = program->uniformLocation("material.ka");
    coefsGPU.kd = program->uniformLocation("material.kd");
    coefsGPU.ks = program->uniformLocation("material.ks");
    coefsGPU.ss = program->uniformLocation("material.shineness");

    coefs.ka = ka;
    coefs.ks = ks;
    coefs.kd = kd;
    coefs.shineness = shinesess;
}

void Material::toGPU(QGLShaderProgram* program)
{
    glUniform1f(coefsGPU.ka, coefs.ka);
    glUniform1f(coefsGPU.kd, coefs.kd);
    glUniform1f(coefsGPU.ks, coefs.ks);
    glUniform1f(coefsGPU.ss, coefs.shineness);
}
