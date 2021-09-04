#include "startwindow.h"

#include <QEvent>
#include <QGraphicsProxyWidget>
#include <QLayout>
#include <QResizeEvent>

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

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent), m_view(nullptr), m_scene(nullptr),
      m_background(nullptr), m_startButton(nullptr) {
    m_view  = new FitGraphicsView();
    m_scene = new QGraphicsScene();
    m_view->setScene(m_scene);
    m_background = new QGraphicsPixmapItem(QPixmap(":/images/cover.jpg"));
    m_scene->addItem(m_background);
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(new QHBoxLayout(this->centralWidget()));
    this->centralWidget()->layout()->addWidget(m_view);

    m_startButton = m_scene->addWidget(new QPushButton());
    // m_startButton->
    dynamic_cast<QPushButton*>(m_startButton->widget())->setText("开始！");
    dynamic_cast<QPushButton*>(m_startButton->widget())->setStyleSheet("font-size:80px");
    m_startButton->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    connect(m_view, &FitGraphicsView::finishPainting, this,
            &StartWindow::setButtonPos);
}

void StartWindow::setButtonPos() {
    auto v = m_startButton->scene()->views().first();
    m_startButton->setPos(v->mapToScene(QPoint(v->width()/2 - 100,v->height() - 100)));
    // m_startButton->
    // m_startButton->setS(QSize(4000,1000));
}

StartWindow::~StartWindow() {
    delete m_view;
    delete m_scene;
}

// bool StartWindow::eventFilter(QObject *object, QEvent *event) {}
