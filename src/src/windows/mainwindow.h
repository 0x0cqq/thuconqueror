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
    Mainwindow(qint32 gamelevel, QWidget *parent);
    ~Mainwindow();
    QHBoxLayout *   hBox;
    QVBoxLayout *   hvBox;
    QLabel *        gameStatusLabel;
    QLabel *        detailLabel;
    QLabel *        policyTreeLabel;
    QMediaPlayer *  play_music;
    QMediaPlaylist *playlist;
    Game *          game;
    void            addGame(QString fileName);
    void            addGame(int gameLevel);
    void addGameLose();
    void            addMusic();
    void            closeEvent(QCloseEvent *event) override;
  signals:
    void mainWindowClosed();
};
#endif  // MAINWINDOW_H
