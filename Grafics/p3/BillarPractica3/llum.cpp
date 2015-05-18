#include "llum.h"
#include <GL/glu.h>
#include "Common.h"

Llum::Llum(QGLShaderProgram* program, int id_, TipusLlum tipus_):
    id(id_),
    tipus(tipus_)
{
    for (int i = 0; i < SHADING_MAX; ++i)
    {
        locationsCached[i] = false;
    }

    direccio = vec4(0, 0, 0, 0);
    angle = 0;
}

Llum::~Llum()
{

}

void Llum::toGPU(QGLShaderProgram* program)
{
    if (!locationsCached[Common::getShadingMode()])
    {
        gl_ldLlum[Common::getShadingMode()].tipus = program->uniformLocation(QString("light[%1].tipus").arg(id));
        gl_ldLlum[Common::getShadingMode()].posicio = program->uniformLocation(QString("light[%1].LightPosition").arg(id));
        gl_ldLlum[Common::getShadingMode()].direccio = program->uniformLocation(QString("light[%1].LightDirection").arg(id));
        gl_ldLlum[Common::getShadingMode()].angle = program->uniformLocation(QString("light[%1].angle").arg(id));
        gl_ldLlum[Common::getShadingMode()].ld = program->uniformLocation(QString("light[%1].Ld").arg(id));
        gl_ldLlum[Common::getShadingMode()].ls = program->uniformLocation(QString("light[%1].Ls").arg(id));
        gl_ldLlum[Common::getShadingMode()].la = program->uniformLocation(QString("light[%1].La").arg(id));
        gl_ldLlum[Common::getShadingMode()].a =  program->uniformLocation(QString("light[%1].coef_a").arg(id));
        gl_ldLlum[Common::getShadingMode()].b =  program->uniformLocation(QString("light[%1].coef_b").arg(id));
        gl_ldLlum[Common::getShadingMode()].c =  program->uniformLocation(QString("light[%1].coef_c").arg(id));

        locationsCached[Common::getShadingMode()] = true;
    }

    glUniform1i(gl_ldLlum[Common::getShadingMode()].tipus, (int)tipus);
    glUniform4fv(gl_ldLlum[Common::getShadingMode()].posicio, 1, posicioLlum);
    glUniform4fv(gl_ldLlum[Common::getShadingMode()].direccio, 1, direccio);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].angle, angle);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].ld, 1, difusa);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].ls, 1, especular);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].la, 1, ambient);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].a, coefA);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].b, coefB);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].c, coefC);
}
