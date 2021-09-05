#include "myapplication.h"

MyApplication::MyApplication(int &argc, char *argv[])
    : QApplication(argc, argv) {
    createStartWindow();
} 

void MyApplication::createStartWindow() {
    startWindow = new StartWindow;
    startWindow->show();
}

void MyApplication::changeToMainWindow() {
    // delete startWindow;
    startWindow->hide();
    mainWindow = new Mainwindow;
    mainWindow->show();
}

MyApplication::~MyApplication() {}
