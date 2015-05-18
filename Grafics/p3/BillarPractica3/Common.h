//////////////////////////////////////////////////////////////////////////////
//
//  --- Angel.h ---
//
//   The main header file for all examples from Angel 6th Edition
//   Renamed to Common.h
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_H__
#define __ANGEL_H__

//----------------------------------------------------------------------------
// 
// --- Include system headers ---
//

#include <cmath>
#include <iostream>
#include <stdlib.h>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

//----------------------------------------------------------------------------
//
// --- Include OpenGL header files and helpers ---
//
//   The location of these files vary by operating system.  We've included
//     copies of open-soruce project headers in the "GL" directory local
//     this this "include" directory.
//

#define GL_GLEXT_PROTOTYPES 1
# include  <GL/gl.h>
# include  <GL/glext.h>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

//----------------------------------------------------------------------------
//
//  --- Include our class libraries and constants ---
//


namespace Common {


//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);

//  Degrees-to-radians constant 
const GLfloat  DegreesToRadians = M_PI / 180.0;

}  // namespace Common

#include "vec.h"
#include "mat.h"

#define Print(x)  do { std::cerr << #x " = " << (x) << std::endl; } while(0)


//  Globally use our namespace in our example programs.
using namespace Common;

typedef struct {
      vec3 pmin;
      vec3 pmax;
      vec3 center;

      mat4 toCenter;
      mat4 fromCenter;
} Capsa3D;

typedef struct {
      vec2 pmin;
      float a, h;
} Capsa2D;


/**
 *Defineix un namespace que conte totes les variables i metodes que ens interesaran consultar durant l'execucio del programa
 * Aixo ens permet unificar nombres, fent aixi mes facil qualsevol canvi.
 */
namespace Common
{
    const GLfloat a = 20;
    const GLfloat h = 20;
    const GLfloat p = 20;

    const vec3 dimensions(a, h, p);
    const vec3 scale(2/a, 2/h, 2/p);

    enum ShadingMode
    {
        FLAT,
        GOURAUD,
        PHONG,
        TOON,
    };

    #define SHADING_MAX 5

    unsigned int getWireframeView();
    void changeViewMode();
    vec3 sceneDimensions();
    vec3 scaleFactor();

    ShadingMode getShadingMode();
    void setShadingMode(ShadingMode shadingMode);
}

#endif // __ANGEL_H__
