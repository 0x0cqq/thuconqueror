#include "myapplication.h"

MyApplication::MyApplication(int &argc, char *argv[])
    : QApplication(argc, argv), startWindow(nullptr), mainWindow(nullptr) {
    createStartWindow();
    // connect(startWindow, &StartWindow::userStartGame, this,
    //         &MyApplication::changeToMainWindow);
    connect(startWindow->m_chooseLevelView, &ChooseLevelView::userStartLevel,
            this, &MyApplication::changeToMainWindow);
}

void MyApplication::createStartWindow() {
    startWindow = new StartWindow;
    startWindow->show();
}

void MyApplication::changeToMainWindow(qint32 gameLevel) {
    // delete startWindow;
    // startWindow->hide();
    if(mainWindow != nullptr) {
        mainWindow->hide();
        mainWindow->deleteLater();
    }
    mainWindow = new Mainwindow(gameLevel);
    mainWindow->show();
    startWindow->hide();
    connect(mainWindow, Mainwindow::mainWindowClosed, this,
            [=]() { this->startWindow->show(); });
}

MyApplication::~MyApplication() {
    delete startWindow;
    delete mainWindow;
}
