#include "startwindow.h"

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

void FitGraphicsView::resizeEvent(QResizeEvent *event) {
    if(this->scene() != nullptr) {
        this->fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    // qDebug() << event->size();
    this->QGraphicsView::resizeEvent(event);
}

void FitGraphicsView::paintEvent(QPaintEvent *event) {
    emit finishPainting();
    QGraphicsView::paintEvent(event);
}

FitGraphicsView::FitGraphicsView() : QGraphicsView(), m_scene(nullptr) {
    m_scene = new QGraphicsScene;
    this->setScene(m_scene);
}

FitGraphicsView::~FitGraphicsView() {
    delete m_scene;
}

StartView::StartView() : m_background(nullptr), m_startButton(nullptr) {
    m_background = new QGraphicsPixmapItem(QPixmap(":/images/cover.jpg"));
    m_scene->addItem(m_background);
    m_startButton = m_scene->addWidget(new QPushButton());
    dynamic_cast<QPushButton *>(m_startButton->widget())->setText("开始！");
    dynamic_cast<QPushButton *>(m_startButton->widget())
        ->setStyleSheet("font-size:80px");
    m_startButton->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    connect(this, &StartView::finishPainting, this, &StartView::setButtonPos);
}

ChooseLevelView::ChooseLevelView() : FitGraphicsView(), m_background(nullptr) {
    m_background = new QGraphicsPixmapItem(QPixmap(":/images/chooselevel.jpg"));
    m_scene->addItem(m_background);
}

void StartView::setButtonPos() {
    auto v = m_startButton->scene()->views().first();
    m_startButton->setPos(
        v->mapToScene(QPoint(v->width() / 2 - 100, v->height() - 100)));
    // m_startButton->
    // m_startButton->setS(QSize(4000,1000));
}

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent), m_startView(nullptr) {
    m_startView       = new StartView();
    m_chooseLevelView = new ChooseLevelView();
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(new QHBoxLayout(this->centralWidget()));
    this->centralWidget()->layout()->addWidget(m_startView);
    connect(
        dynamic_cast<QPushButton *>(this->m_startView->m_startButton->widget()),
        &QPushButton::clicked, this, &StartWindow::changeToChooseLevelWindow);
    connect(
        dynamic_cast<QPushButton *>(this->m_startView->m_startButton->widget()),
        &QPushButton::clicked, this, &StartWindow::userStartGame);
    // m_startButton->
}

StartWindow::~StartWindow() {
    delete m_startView;
}

void StartWindow::changeToChooseLevelWindow() {
    this->centralWidget()->layout()->removeWidget(m_startView);
    m_startView->hide();
    this->centralWidget()->layout()->addWidget(m_chooseLevelView);
}

// bool StartWindow::eventFilter(QObject *object, QEvent *event) {}