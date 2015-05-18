#include "material.h"

Material::Material(QGLShaderProgram* program, int id, vec3 ka, vec3 kd, vec3 ks, float shinesess, vec4 color)
{
    for (int i = 0; i < SHADING_MAX; ++i)
    {
        locationsCached[i] = false;
    }

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

    if (!locationsCached[Common::getShadingMode()])
    {
        coefsGPU[Common::getShadingMode()].ka = program->uniformLocation(QString("material[%1].ka").arg(id));
        coefsGPU[Common::getShadingMode()].kd = program->uniformLocation(QString("material[%1].kd").arg(id));
        coefsGPU[Common::getShadingMode()].ks = program->uniformLocation(QString("material[%1].ks").arg(id));
        coefsGPU[Common::getShadingMode()].ss = program->uniformLocation(QString("material[%1].shineness").arg(id));
        coefsGPU[Common::getShadingMode()].color = program->uniformLocation(QString("material[%1].color").arg(id));

        locationsCached[Common::getShadingMode()] = true;
    }

    glUniform3fv(coefsGPU[Common::getShadingMode()].ka, 1, coefs.ka);
    glUniform3fv(coefsGPU[Common::getShadingMode()].kd, 1, coefs.kd);
    glUniform3fv(coefsGPU[Common::getShadingMode()].ks, 1, coefs.ks);
    glUniform1f(coefsGPU[Common::getShadingMode()].ss, coefs.shineness);
    glUniform4fv(coefsGPU[Common::getShadingMode()].color, 1, coefs.color);
}
