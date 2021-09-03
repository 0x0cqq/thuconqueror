#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <cstdlib>

Mainwindow::Mainwindow(QWidget *parent) : QMainWindow(parent) {
    this->setCentralWidget(new QWidget(this));
    hBox      = new QHBoxLayout(this->centralWidget());
    graphView = new GraphView();
    hBox->addWidget(graphView);
    hvBox = new QVBoxLayout();
    hBox->addLayout(hvBox);
    // buttonBox = new QHBoxLayout();
    // hvBox->addLayout(buttonBox);
    // exitButton = new QPushButton("退出");
    // saveButton = new QPushButton("保存");
    // newButton  = new QPushButton("新建单元");

    // buttonBox->addWidget(saveButton);
    // buttonBox->addWidget(exitButton);
    // buttonBox->addWidget(newButton);

    auto it = new QLabel("滚轮缩放地图，右键点击可以拖动地图。\n移动范围目前是 5 ，攻击范围是 1 。移动、攻击次数均不限。\n");
    gameStatusLabel = new QLabel("游戏信息占位");
    detailLabel     = new QLabel("详细信息占位");
    policyTreeLabel = new QLabel("政策树占位");

    hvBox->addWidget(it);
    hvBox->addWidget(gameStatusLabel);
    hvBox->addWidget(detailLabel);
    hvBox->addWidget(policyTreeLabel);

    // QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    // proxy->setWidget(nextTurnButton);

    // hvBox->addWidget(nextTurnButton);

    this->resize(1920, 1080);

    // add scene
    game = new Game(graphView, QPoint(24, 16), this);
    game->setNewUnitButton();
    game->setNextTurnButton();
    game->setgameStatusLabel(gameStatusLabel);
    game->setDetailedLabel(detailLabel);
    game->init();
}

Mainwindow::~Mainwindow() {}
