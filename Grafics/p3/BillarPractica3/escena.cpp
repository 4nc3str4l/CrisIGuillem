#include "escena.h"
#include "camera.h"
#include "llum.h"

/*
 * Permet pasar el tamany de l'ecena com a parametre,
 * */
Escena::Escena(vec3 dimensions, QGLShaderProgram* program)
{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin.x = -dimensions.x/2; capsaMinima.pmin.y =-dimensions.y/2; capsaMinima.pmin.z = -dimensions.z/2;
    capsaMinima.pmax.x = dimensions.x/2; capsaMinima.pmax.y = dimensions.y/2; capsaMinima.pmax.z = dimensions.z/2;
    //capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;
    scale = Scale(2/dimensions.x, 2/dimensions.y, 2/dimensions.z);

    //Instanciate a canmera and modify it's atributes.
    camGeneral = new Camera(program);
    camFP = new Camera(program);

    // Per defecte totes les llums
    numLlums = 3;

    // Crear llum1
    {
        Llum* llum = new Llum(program, 0, PUNTUAL);
        llum->setCoeficients(0.1, 0.1, 0.1);

        llum->setDifusa(vec3(1.0, 1.0, 1.0));
        llum->setEspecular(vec3(1.0, 1.0, 1.0));
        llum->setAmbient(vec3(1.0, 1.0, 1.0));

        llum->setPosicioLlum(vec4(0, 0, 0, 1));

        // Send to GPU
        llum->toGPU(program);

        llums.push_back(llum);
    }

    // Crear llum2
    {
        Llum* llum = new Llum(program, 1, DIRECCIONAL);
        llum->setCoeficients(0.1, 0.5, 0);

        llum->setDifusa(vec3(0.5, 0.5, 0.5));
        llum->setEspecular(vec3(0.5, 0.5, 0.5));
        llum->setAmbient(vec3(0.5, 0.5, 0.5));

        llum->setPosicioLlum(vec4(4.5, 0, 0, 1));

        llum->setDireccio(vec4(0, 0, 0, 1) - vec4(4.5, 0, 0, 1));

        // Send to GPU
        llum->toGPU(program);

        llums.push_back(llum);
    }

    // Crear llum3
    {
        Llum* llum = new Llum(program, 2, SPOTLIGHT);
        llum->setCoeficients(0.1, 0.1, 0.1);

        llum->setDifusa(vec3(0.3, 0.3, 0.3));
        llum->setEspecular(vec3(0.3, 0.3, 0.3));
        llum->setAmbient(vec3(0.3, 0.3, 0.3));

        llum->setPosicioLlum(vec4(2.5, 2, 1, 1));

        llum->setDireccio(vec4(2.5, 1, -2.5, 1) - vec4(2.5, 5, 1, 1));
        llum->setAngle(10.0f);

        // Send to GPU
        llum->toGPU(program);

        llums.push_back(llum);
    }
}


/*
 * Com tots els objectes formen part de l'escena o son fills de l'escena els destruim iterant sobre ells.
 * I recursivament això eliminarà als fills.
 * */
Escena::~Escena()
{
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        delete (*it);
    }
}

//Afegir un objecte a l'escena.
void Escena::addObjecte(Objecte *obj) {
    objectes.push_back(obj);
}

/* Aquest metode no cal implementar-lo ja que la capça es calcula un cop en el costructor
*/
void Escena::CapsaMinCont3DEscena()
{
}

void Escena::toGPU(QGLShaderProgram* program)
{
    setAmbientToGPU(program);

    for (std::vector<Llum*>::iterator it = llums.begin(); it != llums.end(); ++it)
    {
        (*it)->toGPU(program);
    }

    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->toGPU(program);
    }
}

/*
 * Crida a tots els objectes de l'escena per aplicar-lis una transformació geometrica.
*/
void Escena::aplicaTG(mat4 m) {

    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTG(m);
    }
}


/*
 * Crida a tots els objectes de l'escena per aplicar-lis una transformació geometrica amb origen al punt (0, 0, 0).
*/
void Escena::aplicaTGCentrat(mat4 m) {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->aplicaTGCentrat(m, &capsaMinima);
    }
}


/*
 * Crida iterativament al metode draw de tots els objectes de l'escena.
 * */
void Escena::draw() {
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        (*it)->draw();
    }
}

/*
 * Serveix per trobar un tipus d'objecte determinat (util per tenir el tauler en temps d'execució per exemple)
 * */
Objecte* Escena::getObjecte(TIPUS_OBJECTE tipus)
{
    for (std::vector<Objecte*>::iterator it = objectes.begin(); it != objectes.end(); ++it)
    {
        if ((*it)->getTipus() == tipus) {
            return (*it);
        }
    }

    return NULL;
}

void Escena::setAmbientToGPU(QGLShaderProgram* program)
{
    // Carregar localitzacio
    if (!locationsCached[Common::getShadingMode()])
    {
        locations[Common::getShadingMode()].ambient = program->uniformLocation("llumAmbient");
        locations[Common::getShadingMode()].numLlums = program->uniformLocation("numLights");

        locationsCached[Common::getShadingMode()] = true;
    }

    glUniform3fv(locations[Common::getShadingMode()].ambient, 1, llumAmbient);
    glUniform1i(locations[Common::getShadingMode()].numLlums, numLlums);
}

void Escena::initCamera(bool camGeneral){
    if(camGeneral)
    {
        this->camGeneral->setProjectionType(PERSPECTIVA);
        this->camGeneral->ini(width, height, this->capsaMinima);
    }
    else
    {
        camFP->setProjectionType(PERSPECTIVA);
        camFP->ini(width, height, this->capsaMinima);
    }
}

void Escena::setAnglesCamera(Camera* camera, float d, float angX, float angY, float angZ)
{
    camera->setObs(camera->CalculObs(camera->getVRP(), d, angX, angY));
    camera->setVUP(camera->CalculVup(angX, angY, angZ));
}

void Escena::setVRPCamera(Camera* camera, point4 vrp)
{
    camera->setVRP(vrp);
}

void Escena::setWindowCamera(Camera* camera, Capsa3D capsa, bool aplicaTransformacions)
{
    vec4 vertex_capsa3d[8];

    //Obtenim els vertex
    camera->VertexCapsa3D(capsa, vertex_capsa3d);

    //multipliquem cada vertex per la model view per tenirlo en coordenades de camera
    for(int i = 0;i<8; i++){
        vertex_capsa3d[i] = camera->getModelView() * vertex_capsa3d[i];
    }

    //Pasem de els punts 3d a punts 2d
    Capsa2D window = camera->CapsaMinCont2DXYVert(vertex_capsa3d, 8);
    setWindowCamera(camera, true, window, aplicaTransformacions);
}

void Escena::setWindowCamera(Camera* camera, bool retallat, Capsa2D window, bool aplicaTransformacions)
{
    camera->CalculWindow(window);

    if (retallat)
    {
        camera->CalculAngleOberturaHoritzontal();
        camera->CalculAngleOberturaVertical();
        camera->CalculWindowAmbRetallat();
    }

    camera->AjustaAspectRatioWd();
    camera->CalculaMatriuProjection();

    // Reaplica pan i zoom
    if (aplicaTransformacions)
    {
        camFP->pan();
        camFP->zoom();
    }
}


void Escena::setDCamera(Camera* camera, float d)
{
    camera->setD(d);
    camera->CalculaMatriuProjection();
}

Camera* Escena::getCamaraGeneral()
{
    return camGeneral;
}

Camera* Escena::getCamaraPrimeraPersona()
{
    return camFP;
}
