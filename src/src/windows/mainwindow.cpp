#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <cstdlib>

Mainwindow::Mainwindow(qint32 gamelevel, QWidget *parent)
    : QMainWindow(parent) {
    this->setCentralWidget(new QWidget(this));
    hBox = new QHBoxLayout(this->centralWidget());
    addGame(gamelevel);
    addMusic();
}

void Mainwindow::addMusic() {
    play_music = new QMediaPlayer(this);
    playlist   = new QMediaPlaylist(this);
    for(int i = 1; i <= 5; i++) {
        playlist->addMedia(
            QUrl("qrc:/audios/short-" + QString::number(i) + ".wav"));
    }
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    play_music->setPlaylist(playlist);
    play_music->play();
}

void Mainwindow::addGame(QString fileName) {
    game = new Game(fileName, this);
    game->init();
    hBox->addWidget(game);
    connect(game, &Game::loadGame, this, [&](QString filename) {
        hBox->removeWidget(game);
        game->deleteLater();
        addGame(filename);
    });
    this->resize(1920, 1080);
    addGameLose();
}

void Mainwindow::addGame(int gameLevel) {
    game = new Game(gameLevel, this);
    game->init();
    hBox->addWidget(game);
    connect(game, &Game::loadGame, this, [&](QString filename) {
        hBox->removeWidget(game);
        game->deleteLater();
        addGame(filename);
    });
    this->resize(1920, 1080);
    addGameLose();
}

void Mainwindow::addGameLose() {
    connect(game, &Game::lose, this, [=](int player) {
        QString text;
        if(player == 1)
            text = "人类";
        else
            text = "病毒";
        QMessageBox *msgbox = new QMessageBox(
            QMessageBox::Information, "游戏结束",
            text +
                "输了!\n是的，疫情防控保卫战不会有胜者。\n窗外传来了广播...("
                "请等 "
                "25 秒)",
            QMessageBox::Ok, this,
            Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                Qt::FramelessWindowHint);
        msgbox->button(QMessageBox::Ok)->setDisabled(true);
        msgbox->show();
        QTimer *      timer = new QTimer;
        QMediaPlayer *w     = new QMediaPlayer;
        if(player == 1)
            w->setMedia(QUrl("qrc:/audios/peoplelose.wav"));
        else
            w->setMedia(QUrl("qrc:/audios/viruslose.wav"));
        play_music->pause();
        w->play();
        timer->singleShot(40 * 1000, [=]() {
            msgbox->button(QMessageBox::Ok)->setDisabled(false);
            timer->deleteLater();
            w->deleteLater();
            this->close();
        });
    });
}

Mainwindow::~Mainwindow() {}

void Mainwindow::closeEvent(QCloseEvent *event) {
    play_music->pause();
    emit mainWindowClosed();
    QMainWindow::closeEvent(event);
}
