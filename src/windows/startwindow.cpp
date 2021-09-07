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
    if(m_background != nullptr) {
        this->fitInView(m_background->boundingRect(), Qt::KeepAspectRatio);
    }
    // qDebug() << event->size();
    this->QGraphicsView::resizeEvent(event);
}

void FitGraphicsView::paintEvent(QPaintEvent *event) {
    emit finishPainting();
    QGraphicsView::paintEvent(event);
}

FitGraphicsView::FitGraphicsView(const QString &filename)
    : QGraphicsView(), m_scene(nullptr) {
    m_scene = new QGraphicsScene;
    this->setScene(m_scene);
    m_background = new QGraphicsPixmapItem(QPixmap(filename));
    m_scene->addItem(m_background);
}

FitGraphicsView::~FitGraphicsView() {
    delete m_scene;
}

StartView::StartView()
    : FitGraphicsView(":/images/cover.jpg"), m_startButton(nullptr) {
    m_startButton = m_scene->addWidget(new QPushButton());
    dynamic_cast<QPushButton *>(m_startButton->widget())->setText("开始！");
    dynamic_cast<QPushButton *>(m_startButton->widget())
        ->setStyleSheet("font-size:80px");
    m_startButton->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    connect(this, &StartView::finishPainting, this, &StartView::setButtonPos);
}

ChooseLevelView::ChooseLevelView()
    : FitGraphicsView(":/images/chooselevel.jpg") {
    QVector<LevelInfo> levels;
    levels.append(
        LevelInfo{1, "决战紫荆之巅",
                  "紫操虽大，但我们已经无处可退：我们的身后就是紫荆一号楼。"});
    levels.append(
        LevelInfo{2, "保卫工字厅", "保卫六教！保卫大礼堂！保卫工字厅！"});
    chooseLevelDialog = m_scene->addWidget(new ChooseDialog(levels));
    chooseLevelDialog->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    connect(dynamic_cast<ChooseDialog *>(chooseLevelDialog->widget()),
            &ChooseDialog::userChooseLevel, this,
            &ChooseLevelView::userStartLevel);
    connect(this, &ChooseLevelView::finishPainting, this,
            &ChooseLevelView::setWidgetPos);
}

void ChooseLevelView::setWidgetPos() {
    auto v = chooseLevelDialog->scene()->views().first();
    // chooseLevelDialog->hide();
    // this->fitInView(m_background->boundingRect(), Qt::KeepAspectRatio);

    if(chooseLevelDialog != nullptr) {
        chooseLevelDialog->setPos(v->mapToScene(
            QPoint(v->width() / 2 - chooseLevelDialog->size().width() / 2,
                   v->height() / 2 - chooseLevelDialog->size().height() / 2)));
        // chooseLevelDialog->show();
    }
}

void StartView::setButtonPos() {
    auto v = m_startButton->scene()->views().first();
    if(m_startButton != nullptr)
        m_startButton->setPos(v->mapToScene(
            QPoint(v->width() / 2 - m_startButton->size().width() / 2,
                   v->height() - m_startButton->size().height())));
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