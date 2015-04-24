/***************************************************/
/*           Definicio de la camera                */
/***************************************************/
#ifndef CAMERA_INCLOS
#define CAMERA_INCLOS
#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

#define PI 3.14159

#define MAXDPOST 100000
#define MINDANT -100000
#define EPS 0.001

#define DEGREES *180.0/PI

typedef enum {VISTA_X = 0, VISTA_Y = 1, VISTA_Z = 2, AXONOMETRIC = 3} TipVista; 

typedef enum {YAMUNT = 0, YABAIX = 1} Orienty; 

typedef enum {PARALLELA = 0, PERSPECTIVA = 1} TipProj; 


typedef struct
 {
  double      angy, angx, angz;  /* angles de gir del sistema de coords obser */
  vec4        vrp;               /* view reference point */
  vec4        obs;               /* posicio de l'observador */
  vec4        vup;               /* verticalitat de la càmera */
 } VisuSystem;

typedef struct  
 {
  TipProj    proj;           /* tipus de proj: 0 paral.lela 1 perspectiva */
  double     d;              /* distancia observador a pla de projeccio */
  double     dant, dpost;    /* distancies al pla de retallat anterior i posterior desde l'observador*/
  double     alfav, alfah;   /* angles d'obertura camera vertical i horitzontal */
} PiramProj;

/* =================================== */
/* =                                 = */
/* =             Camera              = */
/* =                                 = */
/* =================================== */

class Camera
{
public:
    explicit Camera(QGLShaderProgram* program);
    ~Camera() {}

    void ini(int a, int h, Capsa3D c);


    void CalculaMatriuModelView();
    void CalculaMatriuProjection();
    void CalculWindow(Capsa2D c);
    void CalculWindowAmbRetallat();

    vec4 CalculObs(vec4 vrp, double d, double angx, double angy);
    vec3 CalculVup(double angx, double angy, double angz);

    void CreaMatDp(mat4 &MDP);
    void AjustaAspectRatioWd();
    void AmpliaWindow(double r);
    void CalculAngleOberturaVertical();
    void CalculAngleOberturaHoritzontal();

    Capsa2D  CapsaMinCont2DXYVert( vec4 *v, int nv);

    void setViewport(int x, int y, int a, int h);
    void setModelViewToGPU(QGLShaderProgram *program, mat4 m);
    void setProjectionToGPU(QGLShaderProgram *program, mat4 p);
    void toGPU(QGLShaderProgram *program);


    void rotate(Capsa3D c);
    void pan();
    void zoom();

    inline void setProjectionType(TipProj tipus)
    {
        piram.proj = tipus;
    }

    inline void setVRP(vec4 vrp)
    {
        vs.vrp = vrp;
    }

    inline void setObs(vec4 obs)
    {
        vs.obs = obs;
    }

    inline void setVUP(vec4 vup){
        vs.vup = vup;
    }

    inline void setD(float d)
    {
        piram.d = d;
    }

    inline vec4 getVRP()
    {
        return vs.vrp;
    }

    inline float getD()
    {
        return piram.d;
    }

    mat4 getModelView(){
        return transpose(this->modView);
    }

    void VertexCapsa3D(Capsa3D capsaMinima, vec4 vaux[8]);


    VisuSystem vs;      /* Sistema de visualitzacio  */
    PiramProj piram;    /* Piramide de visualitzacio */
    Capsa2D wd;	      /* Window                    */
    Capsa2D vp;       /* Viewport                  */


private:


    mat4  modView; // Matriu model-view de la CPU
    mat4  proj;  // Matriu projection de la CPU
    GLuint  model_view;  // model-view matrix uniform shader variable (GPU)
    GLuint  projection;  // projection matrix uniform shader variable (GPU)
};


#endif









