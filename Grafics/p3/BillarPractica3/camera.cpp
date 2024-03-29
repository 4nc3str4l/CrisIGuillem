#include "camera.h"
#include <GL/glu.h>
#include "Common.h"

Camera::Camera(QGLShaderProgram* program)
{
    cameraZoom = 0;
    cameraPanX = 0;
    cameraPanY = 0;

    vs.vrp = vec4(0.0, 0.0, 0.0, 1.0);
    vs.vup = vec4(0.0, 1.0, 0.0, 0.0);
    vs.obs = vec4(0.0, 0.0, 200.0, 1.0);

    vs.angx = 0;
    vs.angy = 0;
    vs.angz = 0;

    vp.a = 600;
    vp.h = 600;
    vp.pmin[0] = 0;
    vp.pmin[1] = 0;

    piram.proj = PERSPECTIVA;
    piram.d = 100;

    for (int i = 0; i < SHADING_MAX; ++i)
    {
        locationCached[i] = false;
    }
}

void Camera::ini(int a, int h, Capsa3D capsaMinima)
{
    vs.vrp = capsaMinima.center;
}



void Camera::toGPU(QGLShaderProgram *program)
{
    if (!locationCached[Common::getShadingMode()])
    {
        locations[Common::getShadingMode()].model_view = program->uniformLocation("model_view");
        locations[Common::getShadingMode()].projection = program->uniformLocation("projection");
        locations[Common::getShadingMode()].cameraGPU = program->uniformLocation("camera");

        locationCached[Common::getShadingMode()] = true;
    }

    setModelViewToGPU(program, modView);
    setProjectionToGPU(program, proj);

    glUniform4fv(locations[Common::getShadingMode()].cameraGPU, 1, vs.obs);
}




// Suposa que les dades d'obs, vrp i vup son correctes en la camera

void Camera::CalculaMatriuModelView()
{
#ifdef DEBUG_COUT
    std::cout << "OBS: "; for (int i = 0; i < 4; ++i) std::cout << vs.obs[i] << "\t"; std::cout << std::endl;
    std::cout << "VRP: "; for (int i = 0; i < 4; ++i) std::cout << vs.vrp[i] << "\t"; std::cout << std::endl;
    std::cout << "VUP: "; for (int i = 0; i < 4; ++i) std::cout << vs.vup[i] << "\t"; std::cout << std::endl;
#endif

    modView = LookAt(vs.obs, vs.vrp, vs.vup);
}

void Camera::CalculaMatriuProjection()
{
#ifdef DEBUG_COUT
    std::cout << "Window (" << wd.pmin.x << "," << wd.pmin.y << ") <" << wd.a << "x" << wd.h << std::endl;
#endif
    if (piram.proj == PERSPECTIVA)
    {
        mat4 mdp(1.0);
        CreaMatDp(mdp);
        proj = Frustum(wd.pmin.x, wd.pmin.x + wd.a, wd.pmin.y, wd.pmin.y + wd.h, piram.dant, piram.dpost) * mdp;
    }
    else
    {
        proj = Ortho(wd.pmin.x, wd.pmin.x + wd.a, wd.pmin.y, wd.pmin.y + wd.h, piram.dant, piram.dpost);
    }
}

//TODO: Intentar interpretar aquest metode.
void Camera::CalculWindow(Capsa2D c)
{
   // CODI A MODIFICAR DURANT LA PRACTICA 2
    
    wd.pmin.x = c.pmin.x;
    wd.pmin.y = c.pmin.y;
    
    wd.a = c.a;
    wd.h = c.h;
}

void Camera::setViewport(int x, int y, int a, int h)
{
    vp.pmin[0] = x;
    vp.pmin[1] = y;
    vp.a = a;
    vp.h = h;
}

void Camera::setModelViewToGPU(QGLShaderProgram *program, mat4 m)
{
#ifdef DEBUG_COUT
    std::cout << "Model" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            std::cout << m[j][i] << "\t";
        std::cout << std::endl;
    }
#endif

    glUniformMatrix4fv(locations[Common::getShadingMode()].model_view, 1, GL_TRUE, &m[0][0]);
}

void Camera::setProjectionToGPU(QGLShaderProgram *program, mat4 p)
{
#ifdef DEBUG_COUT
    std::cout << "Perspective" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            std::cout << p[j][i] << "\t";
        std::cout << std::endl;
    }
#endif

    glUniformMatrix4fv(locations[Common::getShadingMode()].projection, 1, GL_TRUE, &p[0][0]);
}

void  Camera::AmpliaWindow(double r)
{
  // Pre r = 1.5 => amplia a 150%
  double na, da;

  na  = wd.a * (1.0 + r);
  da = na - wd.a;
  wd.pmin[0] = wd.pmin[0] - 0.5*da;
  wd.a  = na;
  na  = wd.h * (1.0 + r);
  da = na - wd.h;
  wd.pmin[1] = wd.pmin[1] - 0.5*da;
  wd.h  = na;
}



// procediment que calcula el semi angle d'obertura vertical
// a partir de l'alcada total del window (h) i la distancia
// a l'observador

void Camera::CalculAngleOberturaVertical()
{
  piram.alfav =  180.0 * atan2(wd.h/2.0, piram.d)/PI;

}

// procediment que calcula el semi angle d'obertura horitzontal
// a partir de l'amplada total del window (a) i la distancia
// a l'observador

void Camera::CalculAngleOberturaHoritzontal()
{

  piram.alfah =  180.0 * atan2(wd.a/2.0, piram.d)/PI;

}

void  Camera::CalculWindowAmbRetallat()
{

  Capsa2D c;

  if (piram.proj == PARALLELA) {
    /* Projeccio paral.lela:
         el window ve donat: amplada o alcada o tots dos. */

    if( fabs(wd.h) <EPS ) {
      c.a = wd.a;
      c.h = ((float)(vp.h)/ (float)(vp.a) ) * wd.a;
    }
    else {
      c.h = wd.h;
      c.a = ((float)(vp.a)/ (float)(vp.h) ) * wd.h;
    }
  }
  else {

    /* Projeccio perspectiva:
         el window ve donat pels angles d'obertura de la camera: el
         vertical, l'horitzontal o tots dos.  */

    if( fabs(piram.alfav) <EPS ) {
      c.a = 2.0  * piram.d * tan( PI*piram.alfah/180.);
      c.h = ((float)(vp.h)/ (float)(vp.a) ) * c.a;
    }
    else {
      if (fabs(piram.alfah) <EPS ) {
        c.h = 2.0  * piram.d * tan( PI*piram.alfav/180.);
        c.a = ((float)(vp.a)/ (float)(vp.h) ) * c.h;
      }
      else {
        c.a = 2.0  * piram.d * tan( PI*piram.alfah/180.);
        c.h = 2.0  * piram.d * tan( PI*piram.alfav/180.);
      }
    }
  }
  c.pmin[0] = -0.5 * c.a;
  c.pmin[1] = -0.5 * c.h;

  wd.pmin[0] = c.pmin[0];
  wd.pmin[1] = c.pmin[1];
  wd.a = c.a;
  wd.h = c.h;
}



// Donat un window i un viewport, amplia el window per tal que el seu
// aspect ratio sigui igual al del viewport

void    Camera::AjustaAspectRatioWd()
{

    double arvp, arwd;

    arvp = ((double) vp.h)/((double)(vp.a));
    arwd = wd.h/wd.a;
    if(arwd > arvp) {
        wd.a = wd.h/arvp;
    }
    else if (arwd <arvp) {
        wd.h = wd.a *arvp;

    }
    wd.pmin[0] = -0.5*wd.a;
    wd.pmin[1] = -0.5*wd.h;
}



// Accio que crea la matriu de deformacio perspectiva

void Camera::CreaMatDp(mat4 &MDP)
{
    MDP[3][2] = -1/piram.d;
}



Capsa2D  Camera::CapsaMinCont2DXYVert( vec4 *v, int nv)
{
    Capsa2D c;
    vec2    pmin, pmax;
    int     i;

    pmin[0] = v[0][0];       pmin[1] = v[0][1];
    pmax[0] = v[0][0];       pmax[1] = v[0][1];
    for(i=1; i<nv; i++) {
      if(v[i][0] <pmin[0])
        pmin[0] = v[i][0];
      if(v[i][1] <pmin[1])
        pmin[1] = v[i][1];
      if(v[i][0] >pmax[0])
        pmax[0] = v[i][0];
      if(v[i][1] >pmax[1])
        pmax[1] = v[i][1];
    }

    c.a = pmax[0]-pmin[0]+1;
    c.h = pmax[1]-pmin[1]+1;
    c.pmin[0] = -c.a/2.0;
    c.pmin[1] = -c.h/2.0;
    return c;
}



//   Calcul del observador:
//   obs es el vrp translladat en la direccio del vector de visio  d'una
//   distancia d = distancia (obs, vrp) si la projeccio es perspectiva
//   i d'una distancia prou gran si la projeccio es paral.lela

vec4 Camera::CalculObs(vec4 vrp, double d, double angx, double angy)
{
 vec4 obs2;
 vec3 v;
 double norma;

 vs.angx = angx;
 vs.angy = angy;

 /* Calcul del vector de visio a partir dels angles */
 v[0] = sin(PI * angy / 180.) * cos(PI * angx / 180.);
 v[2] = cos(PI * angy / 180.) * cos(PI * angx / 180.);
 v[1]= - sin(PI * angx / 180.);

 norma = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);

 v[0] = v[0]/norma;
 v[1] = v[1]/norma;
 v[2] = v[2]/norma;


 obs2[0] = vrp[0] + v[0] *d;
 obs2[1] = vrp[1] + v[1] *d;
 obs2[2] = vrp[2] + v[2] *d;
 obs2[3] = 1.0;

 return(obs2);

}


vec3 Camera::CalculVup(double angx, double angy, double angz)
{
  vec3 v;
  int   x, y;

  x = 1.0;
  y = 1.0;

  if (cos(PI*angx/180.)<0.0) y = -1.0;

  if (cos(PI*angy/180.)<0.0) x = -1.0;


  v[0] = x*sin (-PI*angz/180.);
  v[1] = y*cos( -PI*angz/180.);
  v[2] = 0.0;

  return(v);

}

void Camera::VertexCapsa3D(Capsa3D capsaMinima, vec4 vaux[8])
{

    vec3 ptfi;

    ptfi[0] = capsaMinima.pmax[0];
    ptfi[1] = capsaMinima.pmax[1];
    ptfi[2] = capsaMinima.pmax[2];

    vaux[0] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], capsaMinima.pmin[2], 1.0);
    vaux[1] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], ptfi[2], 1.0);
    vaux[2] = vec4(capsaMinima.pmin[0],  ptfi[1], capsaMinima.pmin[2],1.0);
    vaux[3] = vec4(capsaMinima.pmin[0], ptfi[1], ptfi[2], 1.0);
    vaux[4] = vec4(ptfi[0],capsaMinima.pmin[1], capsaMinima.pmin[2],1.0);
    vaux[5] = vec4(ptfi[0], capsaMinima.pmin[1], ptfi[2],1.0);
    vaux[6] = vec4(ptfi[0], ptfi[1], capsaMinima.pmin[2], 1.0);
    vaux[7] = vec4(ptfi[0], ptfi[1], ptfi[2], 1.0);
}
