/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Play;
    QAction *action_Pla_Base;
    QAction *action_Bola_Blanca;
    QAction *action_15_Boles;
    QAction *action_Obre_Fitxer;
    QAction *action_Sala_de_Billar;
    QAction *action_Exit;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuModels;
    QMenu *menuPlay;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        action_Play = new QAction(MainWindow);
        action_Play->setObjectName(QStringLiteral("action_Play"));
        action_Pla_Base = new QAction(MainWindow);
        action_Pla_Base->setObjectName(QStringLiteral("action_Pla_Base"));
        action_Bola_Blanca = new QAction(MainWindow);
        action_Bola_Blanca->setObjectName(QStringLiteral("action_Bola_Blanca"));
        action_15_Boles = new QAction(MainWindow);
        action_15_Boles->setObjectName(QStringLiteral("action_15_Boles"));
        action_Obre_Fitxer = new QAction(MainWindow);
        action_Obre_Fitxer->setObjectName(QStringLiteral("action_Obre_Fitxer"));
        action_Sala_de_Billar = new QAction(MainWindow);
        action_Sala_de_Billar->setObjectName(QStringLiteral("action_Sala_de_Billar"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        menuModels = new QMenu(menuBar);
        menuModels->setObjectName(QStringLiteral("menuModels"));
        menuPlay = new QMenu(menuBar);
        menuPlay->setObjectName(QStringLiteral("menuPlay"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuModels->menuAction());
        menuBar->addAction(menuPlay->menuAction());
        menuModels->addAction(action_Pla_Base);
        menuModels->addAction(action_Bola_Blanca);
        menuModels->addAction(action_15_Boles);
        menuModels->addAction(action_Obre_Fitxer);
        menuModels->addAction(action_Sala_de_Billar);
        menuModels->addSeparator();
        menuModels->addAction(action_Exit);
        menuPlay->addAction(action_Play);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action_Play->setText(QApplication::translate("MainWindow", "Play", 0));
        action_Pla_Base->setText(QApplication::translate("MainWindow", "Afegeix Pla Base", 0));
        action_Bola_Blanca->setText(QApplication::translate("MainWindow", "Afegeix Bola Blanca", 0));
        action_15_Boles->setText(QApplication::translate("MainWindow", "Afegeix 15 Boles", 0));
        action_Obre_Fitxer->setText(QApplication::translate("MainWindow", "Obre Fitxer", 0));
#ifndef QT_NO_TOOLTIP
        action_Obre_Fitxer->setToolTip(QApplication::translate("MainWindow", "Obre Fitxer", 0));
#endif // QT_NO_TOOLTIP
        action_Obre_Fitxer->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        action_Sala_de_Billar->setText(QApplication::translate("MainWindow", "Afegeix Sala de Billar", 0));
        action_Exit->setText(QApplication::translate("MainWindow", "Exit", 0));
        menuModels->setTitle(QApplication::translate("MainWindow", "Models", 0));
        menuPlay->setTitle(QApplication::translate("MainWindow", "Controls", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
