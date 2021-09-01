#include "mainwindow.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QPushButton>

Mainwindow::Mainwindow(QWidget *parent) : QMainWindow(parent) {
    this->setCentralWidget(new QWidget(this));
    hBox      = new QHBoxLayout(this->centralWidget());
    graphView = new GraphView();
    hBox->addWidget(graphView);
    hvBox = new QVBoxLayout();
    hBox->addLayout(hvBox);
    buttonBox = new QHBoxLayout();
    hvBox->addLayout(buttonBox);
    exitButton = new QPushButton();
    exitButton->setText("退出");

    saveButton = new QPushButton();
    saveButton->setText("保存");

    newButton = new QPushButton();
    newButton->setText("新建单元");

    buttonBox->addWidget(saveButton);
    buttonBox->addWidget(exitButton);
    buttonBox->addWidget(newButton);

    gameStatusLabel = new QLabel();
    gameStatusLabel->setText("游戏信息占位");

    detailLabel = new QLabel();
    detailLabel->setText("详细信息占位");
    policyTreeLabel = new QLabel();
    policyTreeLabel->setText("政策树占位");
    hvBox->addWidget(gameStatusLabel);
    hvBox->addWidget(detailLabel);
    hvBox->addWidget(policyTreeLabel);
    nextTurnButton = new QPushButton("下一回合");
    hvBox->addWidget(nextTurnButton);
    this->resize(1920, 1080);

    // add scene
    game = new Game(graphView, QPoint(24, 16), this);
    game->setNewUnitButton(newButton);
    game->setgameStatusLabel(gameStatusLabel);
    game->setNextTurnButton(nextTurnButton);
    game->init();
    game->setDetailedLabel(detailLabel);
}

Mainwindow::~Mainwindow() {}
