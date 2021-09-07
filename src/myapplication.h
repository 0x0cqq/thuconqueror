#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "windows/mainwindow.h"
#include "windows/startwindow.h"
#include <QApplication>


class MyApplication : public QApplication {
    Q_OBJECT
    StartWindow *startWindow;
    Mainwindow * mainWindow;
  public:
    MyApplication(int &argc, char *argv[]);
    ~MyApplication();
  public slots:
    void createStartWindow();
    void changeToMainWindow(qint32 gameLevel);

};

#endif
