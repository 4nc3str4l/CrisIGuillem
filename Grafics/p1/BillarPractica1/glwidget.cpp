

#include <math.h>

#include <glwidget.h>
#include <QString>

#include "plabase.h"
#include "bola.h"



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)

{
    setFocusPolicy( Qt::StrongFocus );
    esc = new Escena();

    xRot = 0;
    yRot = 0;
    zRot = 0;

    a = 20.0;
    h = 20.0;
    p = 20.0;

    clearColor = Qt::black;
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
    std::cout << "Em crido" << std::endl;
    Common::changeViewMode();
    updateGL();

}



void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj)
{
    Capsa3D& capsa = obj->capsa;
    vec3 nouCentre = vec3(capsa.center.x*(2/a),capsa.center.y*(2/h), capsa.center.z*(2/p));

    mat4 tg = Translate(nouCentre) * Scale(2/a,2/h,2/p) * capsa.toCenter;
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
    Objecte* plaBase = new PlaBase();
    plaBase->toGPU(program);
    esc->addObjecte(plaBase);
    adaptaObjecteTamanyWidget(plaBase);
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
    Objecte* bola = new Bola(vec3(1,1,1));
    bola->toGPU(program);
    esc->addObjecte(bola);
    adaptaObjecteTamanyWidget(bola);
}
void GLWidget::newConjuntBoles()
{
    // Metode que crea les 15 Boles del billar america
    // Metode a implementar

}
void GLWidget::newSalaBillar()
{
    // Metode que construeix tota la sala de billar: taula, 15 boles i bola blanca
    newObj("/home/guillem/Git/CrisIGuillem/Grafics/p1/BillarPractica1/resources/taula.obj");
}

// Metode per iniciar la dinàmica del joc
void GLWidget::Play()
{
    newSalaBillar();
}
