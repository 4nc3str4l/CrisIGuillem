#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <math.h>
#include <escena.h>

#include <QtWidgets>
#include <QGLWidget>
#include <QTime>
#include <map>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1
#define PROGRAM_TEXTURE_ATTRIBUTE 2

class QGLShaderProgram;
class Camera;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void Pan(float dx, float dy);

public slots:
    void newPlaBase();
    void newObj(QString fichero);
    void newBola();
    void newConjuntBoles();
    void newSalaBillar();

    // Metode per iniciar la dinàmica del joc
    void Play();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


private:
    Escena *esc;
    QTimer* timer;
    QTimer* timer_camera;
    quint64 _startTime;

    bool rotar;
    int xRot;
    int yRot;
    int zRot;

    QColor clearColor;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    std::vector<QOpenGLTexture*> textures;

    // Programa de la GPU
    std::map<Common::ShadingMode, QGLShaderProgram*> programs;


    //  Metode per a carregar de fitxers el vertex i el fragment shader
    QGLShaderProgram* InitShader( const char* vertexShaderFile,
                    const char* fragmentShaderFile );

    // Metode per inicialitzar els shaders de l'aplicacio
    void initShadersGPU();

    QGLShaderProgram* currentProgram()
    {
        return programs[Common::getShadingMode()];
    }

    // Metode que carrega un nou objecte a l'escena
    void newObjecte(Objecte * obj);

    // Metode per adaptar l'escena al tamany del widget
    void adaptaObjecteTamanyWidget(Objecte *obj);
    void initTexture(GLenum texture, std::string path);


    Camera* camGeneral;
    Camera* camFP;
    Camera* camActual;

    vec3 deltaSpeed;
    vec2 rotAngle;

    int camera_moves;
    vec4 camera_advance;
    vec4 auxCamObs;
    vec4 auxCamVRP;

    std::map<QGLShaderProgram*, std::pair<GLuint, bool> > only_color;

private slots:
    void mouBola();
    void cameraTransition();

};


#endif // GLWIDGET_H
