#include "myapplication.h"
#include <QRandomGenerator>

MyApplication::MyApplication(int &argc, char *argv[])
    : QApplication(argc, argv), startWindow(nullptr), mainWindow(nullptr) {
    createStartWindow();
    welcomelist   = new QMediaPlaylist(this);
    playlist      = new QMediaPlaylist(this);
    welcome_music = new QMediaPlayer(this);
    play_music    = new QMediaPlayer(this);
    for(int i = 1; i <= 4; i++) {
        playlist->addMedia(
            QUrl("qrc:/audios/short-" + QString::number(i) + ".wav"));
    }
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    welcomelist->addMedia(QUrl("qrc:/audios/1.wav"));
    welcomelist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    play_music->setPlaylist(playlist);
    welcome_music->setPlaylist(welcomelist);
    welcome_music->play();
}

void MyApplication::createStartWindow() {
    startWindow = new StartWindow;
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
    mainWindow = new Mainwindow(gameLevel);
    mainWindow->show();
    startWindow->hide();
    connect(mainWindow, Mainwindow::mainWindowClosed, this, [=]() {
        this->startWindow->show();
        this->play_music->pause();
        this->welcome_music->play();
    });
    welcome_music->pause();
    play_music->play();
}

MyApplication::~MyApplication() {
    delete startWindow;
    delete mainWindow;
}
