#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../game/game.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>

class Mainwindow : public QMainWindow {
    Q_OBJECT

  public:
    Mainwindow(qint32 gamelevel, QWidget *parent = nullptr);
    ~Mainwindow();
    QHBoxLayout *   hBox;
    QVBoxLayout *   hvBox;
    QLabel *        gameStatusLabel;
    QLabel *        detailLabel;
    QLabel *        policyTreeLabel;
    GraphView *     graphView;
    QMediaPlayer *  play_music;
    QMediaPlaylist *playlist;
    Game *          game;
    void            addGame(int gameLevel);
    void            addMusic();
    void            closeEvent(QCloseEvent *event) override;
  signals:
    void mainWindowClosed();
};
#endif  // MAINWINDOW_H
