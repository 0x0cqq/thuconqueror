#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "windows/mainwindow.h"
#include "windows/startwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPLaylist>
#include <QTimer>


class MyApplication : public QApplication {
    Q_OBJECT
    StartWindow *startWindow;
    Mainwindow * mainWindow;
    QMediaPlaylist *welcomelist;

    QMediaPlaylist *playlist;
    QMediaPlayer *welcome_music;
    QMediaPlayer *play_music;
    // QTimer *timer;
  public:
    MyApplication(int &argc, char *argv[]);
    ~MyApplication();
  public slots:
    void createStartWindow();
    void changeToMainWindow(qint32 gameLevel);

};

#endif
