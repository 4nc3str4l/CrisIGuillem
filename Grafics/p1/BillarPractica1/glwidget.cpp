

#include <math.h>

#include <glwidget.h>
#include <QString>
#include <QMessageBox>

#include "plabase.h"
#include "bola.h"
#include "conjuntboles.h"


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    setFocusPolicy( Qt::StrongFocus );

    esc = new Escena(Common::sceneDimensions());

    xRot = 0;
    yRot = 0;
    zRot = 0;


    clearColor = QColor::fromRgbF(0.4, 0.4, 0.4);
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    program = 0;
    moviment = false;

}


GLWidget::~GLWidget()
{
    makeCurrent();
    delete esc;
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

   initTexture(GL_TEXTURE1,"://resources/Bola1.jpg",1);
   initTexture(GL_TEXTURE2,"://resources/Bola2.jpg",2);
   initTexture(GL_TEXTURE3,"://resources/Bola3.jpg",3);
   initTexture(GL_TEXTURE4,"://resources/Bola4.jpg",4);
   initTexture(GL_TEXTURE5,"://resources/Bola5.jpg",5);
   initTexture(GL_TEXTURE6,"://resources/Bola6.jpg",6);
   initTexture(GL_TEXTURE7,"://resources/Bola7.jpg",7);
   initTexture(GL_TEXTURE8,"://resources/Bola8.jpg",8);
   initTexture(GL_TEXTURE9,"://resources/Bola9.jpg",9);
   initTexture(GL_TEXTURE10,"://resources/Bola10.jpg",10);
   initTexture(GL_TEXTURE11,"://resources/Bola11.jpg",11);
   initTexture(GL_TEXTURE12,"://resources/Bola12.jpg",12);
   initTexture(GL_TEXTURE13,"://resources/Bola13.jpg",13);
   initTexture(GL_TEXTURE14,"://resources/Bola14.jpg",14);
   initTexture(GL_TEXTURE0,"://resources/Bola0.jpg",0);
   initTexture(GL_TEXTURE15,"://resources/Fabric_Green_L.jpg", 15);

}

void GLWidget::initTexture(GLenum id, std::string path, int i){

    glActiveTexture(id);
    QOpenGLTexture* texture = new QOpenGLTexture(QImage(path.c_str()));
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    texture->bind(0);

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

    std::cout<<"Estic inicialitzant el shaders"<<std::ends;
    initShadersGPU();

    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GLWidget::paintGL()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   qNormalizeAngle(xRot);
   qNormalizeAngle(yRot);
   qNormalizeAngle(zRot);

   mat4 transform = ( RotateX( xRot / 16.0 ) *
                       RotateY( yRot / 16.0 ) *
                       RotateZ( zRot / 16.0 ) );

   // A modificar si cal girar tots els objectes
   esc->aplicaTGCentrat(transform);
   esc->draw();
}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#else
    glOrtho(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
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
    // Metode a implementar
    switch ( event->key() )
    {
    case Qt::Key_Up:

        break;
    case Qt::Key_Down:

        break;
    case Qt::Key_Left:

        break;
    case Qt::Key_Right:

        break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Metode a implementar en el cas que es mogui la bola
    Common::changeViewMode();
    updateGL();

}



void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{
    Capsa3D& capsa = obj->capsa;
    Capsa3D caixa = esc->getCapsaMinima();
    vec3 nouCentre = vec3(capsa.center.x/caixa.pmax.x,capsa.center.y/caixa.pmax.y, capsa.center.z/caixa.pmax.z);

    mat4 tg = Translate(nouCentre) * esc->getScale() * capsa.toCenter;
    obj->aplicaTG(tg);

    obj->calculCapsa3D();
}

void GLWidget::newObjecte(Objecte * obj)
{
    obj->toGPU(program);
    esc->addObjecte(obj);

    updateGL();
}
void GLWidget::newPlaBase()
{
    Objecte* tauler = esc->getObjecte(TAULER);
    if (!tauler) {
        QMessageBox::warning(NULL, "Falta tauler", "Per favor afegeix un tauler abans d'afegir el pla base");
        return;
    }

    Objecte* plaBase = new PlaBase(tauler);
    plaBase->toGPU(program, *(textures.end() - 1));

    adaptaObjecteTamanyWidget(plaBase);

    vec3 scaleFactor = Common::scaleFactor();
    vec3 tamanyTauler((tauler->capsa.pmax.x - tauler->capsa.pmin.x), 1, (tauler->capsa.pmax.z - tauler->capsa.pmin.z));
    // 20 es el tamany del pla base
    vec3 escalatReal(tamanyTauler.x / 2.34, 1, tamanyTauler.z / 2.16);

    plaBase->aplicaTGCentrat(Scale(escalatReal) * RotateY(180));
    plaBase->aplicaTG(Translate(tauler->capsa.center.x + 0.005, tauler->capsa.center.y+0.08, tauler->capsa.center.z));

    tauler->addChild(plaBase);
}

void GLWidget::newObj(QString fichero)
{
    // Metode que carrega un fitxer .obj llegit de disc
    TaulaBillar *obj;

    obj = new TaulaBillar(fichero);
    newObjecte(obj);
    adaptaObjecteTamanyWidget(obj);
}

void GLWidget::newBola()
{
    Objecte* tauler = esc->getObjecte(TAULER);
    if (!tauler) {
        QMessageBox::warning(NULL, "Falta tauler", "Per favor afegeix un tauler abans d'afegir boles");
        return;
    }

    Objecte* bola = new Bola(vec3(1,1,1));
    bola->toGPU(program, *(textures.end() - 2));


    const GLfloat ballTableRelation = 12;
    vec3 scaleFactor = Common::scaleFactor();
    GLfloat w = (tauler->capsa.pmax.x - tauler->capsa.pmin.x) / ballTableRelation;
    mat4 scaleMatrix = Scale(w, w, w);

    bola->aplicaTG(Translate(tauler->capsa.center.x * scaleFactor.x,
                             1 + tauler->capsa.center.y * scaleFactor.y,
                             25 / ballTableRelation + tauler->capsa.center.z * scaleFactor.z));
    bola->calculCapsa3D();
    bola->aplicaTGCentrat(scaleMatrix);


    tauler->addChild(bola);
    adaptaObjecteTamanyWidget(bola);
}
void GLWidget::newConjuntBoles()
{
    Objecte* tauler = esc->getObjecte(TAULER);
    if (!tauler) {
        QMessageBox::warning(NULL, "Falta tauler", "Per favor afegeix un tauler abans d'afegir boles");
        return;
    }

    ConjuntBoles* boles = new ConjuntBoles(program, tauler, textures);
    tauler->addChild((Objecte*)boles);

    for (int i = 0; i < 15; ++i) {
        adaptaObjecteTamanyWidget(boles->getBola(i));
    }

    /*
    mat4 transform = ( RotateX( xRot / 16.0 ) *
                        RotateY( yRot / 16.0 ) *
                        RotateZ( zRot / 16.0 ) );
    boles->aplicaTGCentrat(transform);
    */
}
void GLWidget::newSalaBillar()
{
    // Metode que construeix tota la sala de billar: taula, 15 boles i bola blanca
    newObj(QDir().current().absoluteFilePath("../BillarPractica1/resources/taula.obj"));
    newPlaBase();
    newConjuntBoles();
    newBola();

    esc->aplicaTGCentrat(Scale(1.5, 1.5, 1.5));
}

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{
    newSalaBillar();
}
