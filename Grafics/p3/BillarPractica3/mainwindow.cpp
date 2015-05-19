
#include <QtWidgets>
#include "ui_mainwindow.h"

#include "glwidget.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("Billar Practica 1: GiVD 2014-2015"));
    resize(640, 640);

    glWidget = new GLWidget;
    setCentralWidget(glWidget);

    //Label explicatiu de com jugar.
    i_label = new QLabel("Pots treure llums amb <b>[I]</b> o posar-ne amb <b>[O]</b>.<br />Per mostrar llums amb textures utiliza <b>[CONTROL]</b> i no [Mayus] juntament amb el número.", glWidget);
    i_label->setStyleSheet("background-color: #fff; color: #000; min-width: 100%;");

    //conecto las distintas opciones de menú con sus metodes
    connect(this->ui->action_Play, SIGNAL(triggered()), glWidget, SLOT(Play()));

    connect(this->ui->action_Sala_de_Billar, SIGNAL(triggered()), glWidget, SLOT(Play()));

    connect(this->ui->action_Exit, SIGNAL(triggered()), this, SLOT(Quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete i_label;
}

void MainWindow::Quit(){
    qApp->exit();
}

void MainWindow::newFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isNull())
        this->glWidget->newObj(fileName);
}
