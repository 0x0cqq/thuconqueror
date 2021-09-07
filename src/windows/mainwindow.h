#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../game/game.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class Mainwindow : public QMainWindow {
    Q_OBJECT

  public:
    Mainwindow(qint32 gamelevel, QWidget *parent = nullptr);
    ~Mainwindow();
    QHBoxLayout *hBox;
    QVBoxLayout *hvBox;
    QLabel *     gameStatusLabel;
    QLabel *     detailLabel;
    QLabel *     policyTreeLabel;
    GraphView *  graphView;
    Game *       game;
    void         closeEvent(QCloseEvent *event) override;
  signals:
    void mainWindowClosed();
};
#endif  // MAINWINDOW_H
