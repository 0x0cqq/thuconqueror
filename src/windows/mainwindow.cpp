#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <cstdlib>

Mainwindow::Mainwindow(qint32 gamelevel, QWidget *parent)
    : QMainWindow(parent) {
    // add scene
    // game = new Game(QPoint(15, 14), this);
    game = new Game(gamelevel);
    game->setNewUnitButton();
    game->setNextTurnButton();
    game->setPolicyTreeButton();
    game->setPauseButton();

    game->setgameStatusLabel();
    game->init();

    this->setCentralWidget(new QWidget(this));
    hBox      = new QHBoxLayout(this->centralWidget());
    graphView = game->m_view;
    hBox->addWidget(graphView);
    this->resize(1920, 1080);
    connect(game, &Game::lose, this, [=](int player) {
        QMessageBox::information(this->centralWidget(), "游戏结束",
                                 "玩家" + QString::number(player) + "输了!");
        this->close();
    });
}

Mainwindow::~Mainwindow() {}

void Mainwindow::closeEvent(QCloseEvent *event) {
    emit mainWindowClosed();
}
