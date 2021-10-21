#include "myapplication.h"
#include <QRandomGenerator>

MyApplication::MyApplication(int &argc, char *argv[])
    : QApplication(argc, argv), startWindow(nullptr), mainWindow(nullptr) {
    createStartWindow();
    welcomelist  = new QMediaPlaylist(this);
    welcomemusic = new QMediaPlayer(this);

    welcomelist->addMedia(QUrl("qrc:/audios/1.wav"));
    welcomelist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    welcomemusic->setPlaylist(welcomelist);
    welcomemusic->play();
}

void MyApplication::createStartWindow() {
    startWindow = new StartWindow(nullptr);
    startWindow->show();
    connect(startWindow, &StartWindow::userChooseLevel, this,
            &MyApplication::changeToMainWindow);
}

void MyApplication::changeToMainWindow(qint32 gameLevel) {
    // delete startWindow;
    // startWindow->hide();
    if(mainWindow != nullptr) {
        mainWindow->hide();
        mainWindow->deleteLater();
    }
    mainWindow = new Mainwindow(gameLevel,nullptr);
    mainWindow->show();
    startWindow->hide();
    connect(mainWindow, Mainwindow::mainWindowClosed, this, [=]() {
        this->startWindow->show();
        this->welcomemusic->play();
    });
    welcomemusic->pause();
}

MyApplication::~MyApplication() {
    delete startWindow;
    delete mainWindow;
}
