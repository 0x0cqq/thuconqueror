#include "mainwindow.h"
#include "graphunit.h"

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

    detailLabel = new QLabel();
    detailLabel->setText("详细信息占位");
    policyTreeLabel = new QLabel();
    policyTreeLabel->setText("政策树占位");
    hvBox->addWidget(detailLabel);
    hvBox->addWidget(policyTreeLabel);
    nextTurnButton = new QPushButton("下一回合");
    hvBox->addWidget(nextTurnButton);
    this->resize(1000, 800);

    // add scene
    graphField = new GraphField;
    connect(newButton, &QPushButton::clicked, graphField, &GraphField::newUnit);
    // auto it = new GraphBlock;
    // it->setPos(100,200);
    // graphField->addItem(it);
    // graphField->addItem(new GraphUnit);
    // auto                itt       = new GraphUnit;
    // graphField->addItem(itt);
    // QPropertyAnimation *animation = new QPropertyAnimation(itt, "pos");
    // animation->setDuration(10000);
    // animation->setStartValue(QPoint(0,0));
    // animation->setEndValue(QPoint(1000,2000));

    // animation->start();

    graphView->setScene(graphField);
}

Mainwindow::~Mainwindow() {}
