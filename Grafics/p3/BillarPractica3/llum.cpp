#include "llum.h"
#include <GL/glu.h>
#include "Common.h"

Llum::Llum(QGLShaderProgram* program, int id_, char* tipus_):
    id(id_),
    tipus(tipus_)
{
    for (int i = 0; i < SHADING_MAX; ++i)
    {
        locationsCached[i] = false;
    }
}

Llum::~Llum()
{

}

void Llum::toGPU(QGLShaderProgram* program)
{
    if (!locationsCached[Common::getShadingMode()])
    {
        gl_ldLlum[Common::getShadingMode()].posicio = program->uniformLocation(QString("%1[%2].LightPosition").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].ld = program->uniformLocation(QString("%1[%2].Ld").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].ls = program->uniformLocation(QString("%1[%2].Ls").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].la = program->uniformLocation(QString("%1[%2].La").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].a =  program->uniformLocation(QString("%1[%2].coef_a").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].b =  program->uniformLocation(QString("%1[%2].coef_b").arg(tipus).arg(id));
        gl_ldLlum[Common::getShadingMode()].c =  program->uniformLocation(QString("%1[%2].coef_c").arg(tipus).arg(id));

        locationsCached[Common::getShadingMode()] = true;
    }

    glUniform4fv(gl_ldLlum[Common::getShadingMode()].posicio, 1, posicioLlum);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].ld, 1, difusa);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].ls, 1, especular);
    glUniform3fv(gl_ldLlum[Common::getShadingMode()].la, 1, ambient);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].a, coefA);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].b, coefB);
    glUniform1f(gl_ldLlum[Common::getShadingMode()].c, coefC);
}
