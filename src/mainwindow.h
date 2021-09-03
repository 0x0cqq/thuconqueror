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
    QHBoxLayout *buttonBox;
    QPushButton *exitButton;
    QPushButton *saveButton;
    QPushButton *newButton;
    QLabel *     gameStatusLabel;
    QLabel *     detailLabel;
    QLabel *     policyTreeLabel;
    QPushButton *nextTurnButton;
    GraphView *  graphView;
    Game *       game;
};
#endif  // MAINWINDOW_H
