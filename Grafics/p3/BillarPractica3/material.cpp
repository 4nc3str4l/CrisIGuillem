#include "material.h"

Material::Material(QGLShaderProgram* program, int id, float ka, float kd, float ks, float shinesess, vec4 color)
{
    coefsGPU.ka = program->uniformLocation(QString("material[%1].ka").arg(id));
    coefsGPU.kd = program->uniformLocation(QString("material[%1].kd").arg(id));
    coefsGPU.ks = program->uniformLocation(QString("material[%1].ks").arg(id));
    coefsGPU.ss = program->uniformLocation(QString("material[%1].shineness").arg(id));
    coefsGPU.color = program->uniformLocation(QString("material[%1].color").arg(id));

    coefs.ka = ka;
    coefs.ks = ks;
    coefs.kd = kd;
    coefs.color = color;
    coefs.shineness = shinesess;
    coefs.color = color;

    this->id = id;
}

void Material::toGPU(QGLShaderProgram* program)
{
    program->bind();

    glUniform1f(coefsGPU.ka, coefs.ka);
    glUniform1f(coefsGPU.kd, coefs.kd);
    glUniform1f(coefsGPU.ks, coefs.ks);
    glUniform1f(coefsGPU.ss, coefs.shineness);
    glUniform4fv(coefsGPU.color, 1, coefs.color);
}
