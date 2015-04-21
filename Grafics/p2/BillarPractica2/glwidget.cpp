

#include <math.h>

#include <glwidget.h>
#include <QString>
#include <QMessageBox>

#include "plabase.h"
#include "bola.h"
#include "conjuntboles.h"
#include "camera.h"


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    setFocusPolicy( Qt::StrongFocus );

    xRot = 0;
    yRot = 0;
    zRot = 0;

    //Fem que el color de borrat sigui un gris.
    clearColor = QColor::fromRgbF(0.4, 0.4, 0.4);
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    //Utilitzat per moure la bola aprentant l'espai.
    timer = new QTimer(this);
    //Linkem el signal timeout amb el metode o slot mou bola.
    connect(timer, SIGNAL(timeout()), this, SLOT(mouBola()));

    program = 0;

    //si cal rotar els objectes de l'escena al apretar una tecla o mouse.
    rotar = true;
}

/**
  Destrueix l'esena, el timer, i els shaders (si estan creats)
 * @brief GLWidget::~GLWidget
 */
GLWidget::~GLWidget()
{
    makeCurrent();
    delete esc;
    delete timer;

    if (program)
    {
        QList<QGLShader*> list = program->shaders();
        for (QList<QGLShader*>::iterator it = list.begin(); it != list.end(); ++it)
        {
            delete *it;
        }

        delete program;
    }

    for (std::vector<QOpenGLTexture*>::iterator it = textures.begin(); it != textures.end(); ++it)
    {
        delete *it;
    }
}


// Create a GLSL program object from vertex and fragment shader files
void
GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile)
{           

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    // Compilació del vertex shader
    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);

    program->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("vColor", PROGRAM_COLOR_ATTRIBUTE);

    // muntatge del shader en el pipeline gràfic per a ser usat
    program->link();

    // unió del shader al pipeline gràfic
    program->bind();
}

void GLWidget::initShadersGPU()
{
// Carrega dels shaders i posa a punt per utilitzar els programes carregats a la GPU
   InitShader( "://vshader1.glsl", "://fshader1.glsl" );

   //Aprofitem per inicilitzar les textures
   initTexture(GL_TEXTURE1,"://resources/Bola1.jpg");
   initTexture(GL_TEXTURE2,"://resources/Bola2.jpg");
   initTexture(GL_TEXTURE3,"://resources/Bola3.jpg");
   initTexture(GL_TEXTURE4,"://resources/Bola4.jpg");
   initTexture(GL_TEXTURE5,"://resources/Bola5.jpg");
   initTexture(GL_TEXTURE6,"://resources/Bola6.jpg");
   initTexture(GL_TEXTURE7,"://resources/Bola7.jpg");
   initTexture(GL_TEXTURE8,"://resources/Bola8.jpg");
   initTexture(GL_TEXTURE9,"://resources/Bola9.jpg");
   initTexture(GL_TEXTURE10,"://resources/Bola10.jpg");
   initTexture(GL_TEXTURE11,"://resources/Bola11.jpg");
   initTexture(GL_TEXTURE12,"://resources/Bola12.jpg");
   initTexture(GL_TEXTURE13,"://resources/Bola13.jpg");
   initTexture(GL_TEXTURE14,"://resources/Bola14.jpg");
   initTexture(GL_TEXTURE0,"://resources/Bola0.jpg");
   initTexture(GL_TEXTURE15,"://resources/Fabric_Green_L.jpg");

}

void GLWidget::initTexture(GLenum id, std::string path){

    //otorga una id a la textura
    glActiveTexture(id);
    // Crea un objecte de qt linkat a aquesta textura.
    QOpenGLTexture* texture = new QOpenGLTexture(QImage(path.c_str()));
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    texture->bind(0);

    //Ens guardem les textures en un vector, per utilitzarles mes endavant.
    textures.push_back(texture);
}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 180;
    while (angle > 360)
        angle -= 180;
}


void GLWidget::setXRotation(int angle)
{
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}


void GLWidget::setYRotation(int angle)
{
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}


void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShadersGPU();

    //Construim l'escena utilitzant el tamanys del common.
    esc = new Escena(Common::sceneDimensions(), program);
    //Obtenim la camera general
    this->camGeneral = esc->getCamaraGeneral();

    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GLWidget::paintGL()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


   //Si ha de rotar doncs, crida a aplica tgcentrat de l'escena que fa que girin tots els objectes de l'escena respecte al centre de la mateixa.
   if (rotar)
   {
       qNormalizeAngle(xRot);
       qNormalizeAngle(yRot);
       qNormalizeAngle(zRot);

       mat4 transform = ( RotateX( xRot / 16.0 ) *
                           RotateY( yRot / 16.0 ) *
                           RotateZ( zRot / 16.0 ) );

       // A modificar si cal girar tots els objectes
       esc->aplicaTGCentrat(transform);
   }
   //Si esta a false es posa a true per la seguent crida
   else
   {
       rotar = true;
   }

   //Mostra els canvis per pantalla.
   esc->draw();
}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    esc->setSize(width, height);
    camGeneral->setViewport((width - side) / 2, (height - side) / 2, side, side);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //declarem el objectes que usarem.
    Objecte* tauler = NULL;
    Objecte* bola = NULL;
    ConjuntBoles* conjuntBoles = NULL;
    PlaBase* plaBase = NULL;

    mat4 transformacions;
    mat4 inversa;

    //si s'han pulsat algunes de les tecles de moviment
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
    {
        //Obtenum el tauler, que te com a fills la bola ,el conjunt de boles i el pla base (tapete).
        tauler = esc->getObjecte(TAULER);

        //obtenim els fills del tauler
        if (tauler) {
            bola = tauler->getFill(BOLA_BLANCA);
            plaBase = (PlaBase*)tauler->getFill(PLA_BASE);
            conjuntBoles = (ConjuntBoles*)tauler->getFill(CONJUNT_BOLES);
        }

        //SI no hi ha fills mostrem un missatge d'error
        if (!bola || !tauler || !plaBase || !conjuntBoles) {
            QMessageBox::warning(NULL, "Atencio", "No tens una taula amb tots els elements.\nPer jugar, tanca, torna a obrir i afegeix sala de billar.");
            return;
        }

        //Obtenim les rotacions acumulades durant l'execució del programa
        transformacions = plaBase->getTransformacions();

        //Agafem la matriu inversa (nomes la calculem quan hi han noves rotacions)
        inversa = plaBase->getInversa();
    }

    //Son matrius de translació precalculades
    static mat4 moveUp = Translate(vec3(0, 0, -0.1));
    static mat4 moveDown = Translate(vec3(0, 0, 0.1));
    static mat4 moveLeft = Translate(vec3(-0.1, 0, 0));
    static mat4 moveRight = Translate(vec3(0.1, 0, 0));

    // Metode a implementar
    switch ( event->key() )
    {
    //si apretem la tecla up
    case Qt::Key_Up:
        //Aplicant la rotacio inversa tornem a posar la taula en la posicio inicial
        tauler->aplicaTGCentrat(inversa);
        //si la bola no esta xocant amb res
        if (intersects(bola->capsa.pmin + vec3(0,0,0.02), bola->capsa.pmax, plaBase->capsa.pmin, plaBase->capsa.pmax) &&
                       !conjuntBoles->collides(bola->capsa.pmin + vec3(0,0,0.01), bola->capsa.pmax))
        {
            //Movem la bola i la rotem sobre si mateixa en la direcció adient
            bola->aplicaTGCentrat(moveDown*RotateX(-30));
        }
        //Tornem el tauler la seva rotació actual.
        tauler->aplicaTGCentrat(transformacions);
        break;

    //Analog al cas anterior
    case Qt::Key_Down:
        tauler->aplicaTGCentrat(inversa);
        if (intersects(bola->capsa.pmin, bola->capsa.pmax  + vec3(0,0,-0.02), plaBase->capsa.pmin, plaBase->capsa.pmax) &&
                !conjuntBoles->collides(bola->capsa.pmin, bola->capsa.pmax  + vec3(0,0,-0.01)))
        {
            bola->aplicaTGCentrat(moveUp*RotateX(30));
        }
        tauler->aplicaTGCentrat(transformacions);
        break;

    //Analog al cas anterior
    case Qt::Key_Left:
        tauler->aplicaTGCentrat(inversa);
        if (intersects(bola->capsa.pmin, bola->capsa.pmax + vec3(-0.02,0,0), plaBase->capsa.pmin, plaBase->capsa.pmax) &&
                       !conjuntBoles->collides(bola->capsa.pmin, bola->capsa.pmax + vec3(-0.01,0,0)))
        {
            bola->aplicaTGCentrat(moveLeft*RotateZ(-30));
        }
        tauler->aplicaTGCentrat(transformacions);
        break;

    //Analog al cas anterior
    case Qt::Key_Right:
        tauler->aplicaTGCentrat(inversa);
        if (intersects(bola->capsa.pmin+ vec3(0.02,0,0), bola->capsa.pmax , plaBase->capsa.pmin, plaBase->capsa.pmax) &&
                        !conjuntBoles->collides(bola->capsa.pmin+ vec3(0.01,0,0), bola->capsa.pmax))
        {
            bola->aplicaTGCentrat(moveRight*RotateZ(30));
        }
        tauler->aplicaTGCentrat(transformacions);
        break;

    //Ens serveix per crear l'efecte de tirar una bola
    case Qt::Key_Space:
    {
        //Si es el primer cop que apretem, guardem els segons d'inici
        if (!event->isAutoRepeat())
        {
            QDateTime time = QDateTime::currentDateTime();
            _startTime = time.toMSecsSinceEpoch();
        }
        break;
    }

    //canviar mode de visualització entre wireframe i faces.
    case Qt::Key_V:
        Common::changeViewMode();
        rotar = false;
        break;
    }


    rotar = false;

    //Actualitzem l'escena i la pantalla.
    updateGL();
}

void GLWidget::mouBola()
{
    //predeclarem les variables
    Objecte* tauler = NULL;
    Objecte* bola = NULL;
    ConjuntBoles* conjuntBoles = NULL;
    PlaBase* plaBase = NULL;

    mat4 transformacions;
    mat4 inversa;

    tauler = esc->getObjecte(TAULER);

    //obtenim els fills del tauler
    if (tauler) {
        bola = tauler->getFill(BOLA_BLANCA);
        plaBase = (PlaBase*)tauler->getFill(PLA_BASE);
        conjuntBoles = (ConjuntBoles*)tauler->getFill(CONJUNT_BOLES);
    }

    //Mostrem un missatge d'error si hem arribat a aquest punt.
    if (!bola || !tauler || !plaBase || !conjuntBoles) {
        QMessageBox::warning(NULL, "Atencio", "No tens una taula amb tots els elements.\nPer jugar, tanca, torna a obrir i afegeix sala de billar.");
        return;
    }

    //Obtenim les rotacions acumulades durant l'execucio del programa
    transformacions = plaBase->getTransformacions();
    // Obtenim la matriu inversa a aquestes rotacions (per poder desfer-les totes de cara a simplicar la gestio del moviment la bola)
    inversa = plaBase->getInversa();

    //Si la velocitat de la bola es 0 parem el timer
    if (bola->speed.z == 0)
    {
        timer->stop();
    }
    //sino incrementem o decrementem la velocitat de la bola aproximant-la a 0.
    else
    {
        char sign = -1;
        if (bola->speed.z < 0)
        {
            sign = 1;
        }

        //Com abans tornem tota l'escena a les rotacions inicials
        tauler->aplicaTGCentrat(inversa);
        //si la bola no xoca amb res
        if (intersects(bola->capsa.pmin + vec3(0,0,0.02), bola->capsa.pmax + vec3(0,0,-0.02), plaBase->capsa.pmin, plaBase->capsa.pmax) &&
                !conjuntBoles->collides(bola->capsa.pmin, bola->capsa.pmax + bola->speed))
        {
            //la movem
            bola->aplicaTGCentrat(Translate(bola->speed)*RotateX(sign*30));
        }
        else
        {
            //canviem el sentit de la seva velocitat
            bola->speed = -bola->speed;
            //la movem
            bola->aplicaTGCentrat(Translate(bola->speed)*RotateX(sign*30));
        }
        //tornem a recuperar les rotacions actuals
        tauler->aplicaTGCentrat(transformacions);

        // Decrementem la velocitat de la bola segons el temps que fa que esta en moviment.
        GLfloat delta = 0.0005;
        if (bola->speed.z > 0)
        {
            delta *= -1;
        }

        if ((delta < 0 && bola->speed.z + delta < 0) ||
                (delta > 0 && bola->speed.z + delta > 0))
        {
            bola->speed.z = 0;
        }
        else
        {
            bola->speed.z += delta;
        }

        rotar = false;

        //actualitzem l'escena i la pantalla.
        updateGL();
    }
}

/**
 * Ens serveix per saber quan l'usuari ha deixat anar la tecla espai (com a maxim la velocitat aumentara durant els 5 primers
 * segons que l'usuari premi la tecla.
 * @brief GLWidget::keyReleaseEvent
 * @param event
 */
void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        QDateTime time = QDateTime::currentDateTime();
        GLfloat total = (time.toMSecsSinceEpoch() - _startTime) / 1000.0;
        if (total > 5)
        {
            total = 5;
        }

        if (!event->isAutoRepeat())
        {
            Objecte* tauler = NULL;
            Objecte* bola = NULL;

            tauler = esc->getObjecte(TAULER);

            if (tauler) {
                bola = tauler->getFill(BOLA_BLANCA);
            }

            if (!bola || !tauler) {
                QMessageBox::warning(NULL, "Atencio", "No tens una taula amb tots els elements.\nPer jugar, tanca, torna a obrir i afegeix sala de billar.");
                return;
            }

            bola->speed = vec3(0,0, total * -0.03);
            timer->start(16);
        }
        else
        {
            cout << "Velocitat acumulada: " << (total * 100 / 5.0) << "%" << endl;
        }
    }
}


/**
 *
 * @brief GLWidget::adaptaObjecteTamanyWidget
 * @param obj
 */
void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{
    //Obtenim les capçes minimen de l'objecte i l'escena
    Capsa3D& capsa = obj->capsa;
    Capsa3D caixa = esc->getCapsaMinima();
    //Guardem el nou centre de l'objecte en un vector que resulta de dividir el centre acutal per l'ample de l'escena entre 2.
    vec3 nouCentre = vec3(capsa.center.x/caixa.pmax.x,capsa.center.y/caixa.pmax.y, capsa.center.z/caixa.pmax.z);

    //Escalem els objectes al tamany de l'escena
    mat4 tg = Translate(nouCentre) * esc->getScale() * capsa.toCenter;

    //Apliquem la transformacio
    obj->aplicaTG(tg);

    //Recalculem la capça 3d de l'objecte ja que al haver-la escalat ha canviat.
    obj->calculCapsa3D();
}

/**
 * Afegeix un nou objecte a l'escena i li otorga uns shaders.
 * @brief GLWidget::newObjecte
 * @param obj
 */
void GLWidget::newObjecte(Objecte * obj)
{
    obj->toGPU(program);
    esc->addObjecte(obj);

    updateGL();
}

/**
 * Afegeix un nou pla base a l'escena
 * @brief GLWidget::newPlaBase
 */
void GLWidget::newPlaBase()
{
    //Instanciem un objecte pla blase
    Objecte* plaBase = new PlaBase();

    //Enviem l'objecte a la gpu amb la seva textura.
    plaBase->toGPU(program, *(textures.end() - 1));

    //Obtenim el tauler per assignar aquest objecte com a fill de tauler (servira per rotar-lo quan el tauler giri).
    Objecte* tauler = esc->getObjecte(TAULER);
    if (!tauler) {
        esc->addObjecte(plaBase);
    }
    //Si hi ha tauler
    else
    {
        //Adaptem el pla base al tamany del tauler ja que l'utilitzem de tapete per la taula.
        vec3 tamanyTauler((tauler->capsa.pmax.x - tauler->capsa.pmin.x), 1, (tauler->capsa.pmax.z - tauler->capsa.pmin.z));

        //Ens serveix per adaptar el tamany del tapete a la taula de billar i quedi be
        vec3 escalatReal(tamanyTauler.x / 23.4, 1, tamanyTauler.z / 21.6);

        // Apliquem les transformacions adients en el tapete per rotarlo i ajustar-lo per quedi visualment consistent
        plaBase->aplicaTGCentrat(Scale(escalatReal) * RotateY(180));
        plaBase->aplicaTG(Translate(tauler->capsa.center.x + 0.05, tauler->capsa.center.y+0.8, tauler->capsa.center.z));

        //Afegim al tapete com a fill de la taula.
        tauler->addChild(plaBase);
    }
}

void GLWidget::newObj(QString fichero)
{
    // Metode que carrega un fitxer .obj llegit de disc
    TaulaBillar *obj;

    obj = new TaulaBillar(fichero);
    newObjecte(obj);
}

void GLWidget::newBola()
{
    //Instanciem una bola de color blanc
    Objecte* bola = new Bola(vec3(1,1,1));
    bola->setTipus(BOLA_BLANCA);

    //Afegim la textura adient a la bola (eviant-la a la gpu amb el seu shader)
    bola->toGPU(program, *(textures.end() - 2));

    //Obtenim el tauler
    Objecte* tauler = esc->getObjecte(TAULER);

    if (!tauler) {
        esc->addObjecte(bola);
    }
    else
    {
        //Pensem que la bola te una relacio 6 amb la taula
        const GLfloat ballTableRelation = 60;
        //Ample de la bola
        GLfloat w = (tauler->capsa.pmax.x - tauler->capsa.pmin.x) / ballTableRelation;
        //matriu d'escala de la bola
        mat4 scaleMatrix = Scale(w, w, w);

        //Movem la bola per posar-la a la taula.
        bola->aplicaTG(Translate(tauler->capsa.center.x,
                                 1 + tauler->capsa.center.y,
                                 150 / ballTableRelation + tauler->capsa.center.z));

        //Calculem la seva capça 3d per si canviat
        bola->calculCapsa3D();

        //Escalem la bola despres de haver-la mogut
        bola->aplicaTGCentrat(scaleMatrix);

        //Afegim la bola com a filla de la taula.
        tauler->addChild(bola);
    }
}

/**
 * Crea el conjunt de boles usant metodes explicats anteriorment
 * @brief GLWidget::newConjuntBoles
 */
void GLWidget::newConjuntBoles()
{
    Objecte* tauler = esc->getObjecte(TAULER);
    ConjuntBoles* boles = new ConjuntBoles(program, tauler, textures);

    if (!tauler) {
        esc->addObjecte(boles);
    }
    else
    {
        tauler->addChild((Objecte*)boles);
    }

}

/**
 * Afegeix tots els elements que formen una sala de billar
 * @brief GLWidget::newSalaBillar
 */
void GLWidget::newSalaBillar()
{
    // Metode que construeix tota la sala de billar: taula, 15 boles i bola blanca
    //Obtenim el path del objecte que defineix la taula
    newObj(":/resources/taula.obj");
    //creem el tapete
    newPlaBase();
    //Creem el conjunt de boles.
    newConjuntBoles();
    //Afegim la bola blanca (la que controla l'usuari)
    newBola();

    Objecte* tauler = esc->getObjecte(TAULER);
    PlaBase* plaBase = NULL;

    if (tauler) {
        plaBase = (PlaBase*)tauler->getFill(PLA_BASE);

        //calculem capsa 3d del tauler
        tauler->calculCapsa3D();
        //declarem una array de vertex per poder guardar els vertex de la capça del tauler
        vec4 vertex_capsa3d[8];
        //Obtenim els vertex
        camGeneral->VertexCapsa3D(tauler->capsa, vertex_capsa3d);
        //multipliquem cada vertex per la model view per tenirlo en coordenades de camera
        for(int i = 0;i<8; i++){
            cout << "Abans : (" << vertex_capsa3d[i].x << ", " << vertex_capsa3d[i].y << ", "<< vertex_capsa3d[i].z << ")" << endl;
            vertex_capsa3d[i] = camGeneral->getModelView() * vertex_capsa3d[i];
            cout << "Despres :(" << vertex_capsa3d[i].x << ", " << vertex_capsa3d[i].y << ", "<< vertex_capsa3d[i].z << ")" << endl;
        }
        //Pasem de els punts 3d a punts 2d
        Capsa2D window = camGeneral->CapsaMinCont2DXYVert(vertex_capsa3d, 8);
        //Calculem la window
        camGeneral->CalculWindow(window);

        camGeneral->CalculaMatriuProjection();
        camGeneral->toGPU(program);

        cout << tauler->capsa.pmin.x <<  ", " <<tauler->capsa.pmin.y << "," <<tauler->capsa.pmin.z << endl;
        cout << tauler->capsa.pmax.x <<  ", " <<tauler->capsa.pmax.y << "," <<tauler->capsa.pmax.z << endl;
    }

    if (!tauler || !plaBase) {
        QMessageBox::warning(NULL, "Atencio", "Algo no ha funcionat be.");
    }

    //A partir d'aquest punt començem a guardar totes les rotacions que es faran a l'escena de cara a poder moure la bola d'un mode mes senzill.
    plaBase->guardaTransformacions();
}

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{
    newSalaBillar();
}
