#include "startwindow.h"
#include "ui_chooselevelwindow.h"
#include "ui_startwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QLayout>
#include <QResizeEvent>
#include <QTextStream>

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent), startview_ui(nullptr), chooselevelview_ui(nullptr) {
    startview_ui       = new Ui::StartWindow();
    chooselevelview_ui = new Ui::ChooseLevelWindow();
    startview_ui->setupUi(this);
    connect(startview_ui->startButton, &QPushButton::clicked, this,
            &StartWindow::changeToChooseLevelWindow);
}

StartWindow::~StartWindow() {
    delete startview_ui;
    delete chooselevelview_ui;
}

void StartWindow::changeToChooseLevelWindow() {
    chooselevelview_ui->setupUi(this);
    connect(chooselevelview_ui->startButton_1, &QPushButton::clicked, this,
            [=]() { emit userChooseLevel(1); });
    connect(chooselevelview_ui->startButton_2, &QPushButton::clicked, this,
            [=]() { emit userChooseLevel(2); });
}

// bool StartWindow::eventFilter(QObject *object, QEvent *event) {}