#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>


#include <Common.h>
#include <objecte.h>
#include <taulabillar.h>
#include <plabase.h>

// Forward declarations
class Camera;
class Llum;

using namespace std;

class Escena
{
public:
    Escena(vec3 dimensions, QGLShaderProgram* program);
    ~Escena();

    void addObjecte(Objecte *obj);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    void CapsaMinCont3DEscena();

    Camera* getCamaraGeneral();
    Camera* getCamaraPrimeraPersona();

    void setAmbientToGPU(QGLShaderProgram* program);

    void setLlumAmbient(vec3 intensitat)
    {
        llumAmbient = intensitat;
    }

    inline Capsa3D getCapsaMinima(){
        return capsaMinima;
    }

    inline mat4 getScale(){
        return scale;
    }

    inline void setSize(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    Objecte* getObjecte(TIPUS_OBJECTE tipus);

    void initCamera(bool camGeneral);
    void setAnglesCamera(Camera* camera,float angX, float angY, float angZ);
    void setVRPCamera(Camera* camera, point4 vrp);
    void setWindowCamera(Camera* camera, Capsa3D capsa);
    void setWindowCamera(Camera* camera, bool retallat, Capsa2D window);
    void setDCamera(Camera* camera, float d);

private:
    std::vector<Objecte*> objectes;
    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;
    mat4 scale;
    Camera* camGeneral;
    Camera* camFP;

    GLuint ambient;
    vec3 llumAmbient;
    Llum* pointLight;

    int width;
    int height;

};

#endif // ESCENA_H
