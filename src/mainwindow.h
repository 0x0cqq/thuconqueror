#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class Mainwindow : public QMainWindow {
    Q_OBJECT

  public:
    Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();
    QHBoxLayout *hBox;
    QVBoxLayout *hvBox;
    QLabel *     gameStatusLabel;
    QLabel *     detailLabel;
    QLabel *     policyTreeLabel;
    GraphView *  graphView;
    Game *       game;
};
#endif  // MAINWINDOW_H
