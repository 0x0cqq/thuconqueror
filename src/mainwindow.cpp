#include "mainwindow.h"

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
    buttonBox->addWidget(saveButton);
    buttonBox->addWidget(exitButton);
    detailLabel = new QLabel();
    detailLabel->setText("详细信息占位");
    policyTreeLabel = new QLabel();
    policyTreeLabel->setText("政策树占位");
    hvBox->addWidget(detailLabel);
    hvBox->addWidget(policyTreeLabel);
    nextTurnButton = new QPushButton("下一回合");
    hvBox->addWidget(nextTurnButton);
    this->resize(1000, 800);
}

Mainwindow::~Mainwindow() {}
