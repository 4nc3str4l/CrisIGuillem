#include "llum.h"
#include <GL/glu.h>
#include "Common.h"

Llum::Llum(QGLShaderProgram* program)
{
    gl_ldLlum.posicio = program->uniformLocation("light.LightPosition");
    gl_ldLlum.ld = program->uniformLocation("light.Ld");
    gl_ldLlum.a =  program->uniformLocation("light.coef_a");
    gl_ldLlum.b =  program->uniformLocation("light.coef_b");
    gl_ldLlum.c =  program->uniformLocation("light.coef_c");

    std::cout << gl_ldLlum.posicio << " "
              << gl_ldLlum.ld << " "
              << gl_ldLlum.a << " "
              << gl_ldLlum.b << " "
              << gl_ldLlum.c << std::endl;
}

Llum::~Llum()
{

}

void Llum::toGPU(QGLShaderProgram* program)
{
    glUniform4fv(gl_ldLlum.posicio, 1, posicioLlum);
    glUniform3fv(gl_ldLlum.ld, 1, difusa);
    glUniform1f(gl_ldLlum.a, coefA);
    glUniform1f(gl_ldLlum.b, coefB);
    glUniform1f(gl_ldLlum.c, coefC);
}
