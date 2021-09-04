#include "myapplication.h"

MyApplication::MyApplication(int &argc, char *argv[])
    : QApplication(argc, argv) {
    createStartWindow();
}

void MyApplication::createStartWindow() {
    startWindow = new StartWindow;
    startWindow->show();
    connect(dynamic_cast<QPushButton *>(startWindow->m_startButton->widget()),
            &QPushButton::clicked, this, &MyApplication::changeToMainWindow);
}

void MyApplication::changeToMainWindow() {
    // delete startWindow;
    startWindow->hide();
    mainWindow = new Mainwindow;
    mainWindow->show();
}

MyApplication::~MyApplication() {}
