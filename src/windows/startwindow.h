#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "../graph/choosedialog.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPushButton>

class FitGraphicsView : public QGraphicsView {
    Q_OBJECT

  public:
    QGraphicsScene *     m_scene;
    QGraphicsPixmapItem *m_background;
    void                 resizeEvent(QResizeEvent *event) override;
    void                 paintEvent(QPaintEvent *event) override;
    FitGraphicsView(const QString &filename);
    ~FitGraphicsView();
  signals:
    void finishPainting();
};

class StartView : public FitGraphicsView {
    Q_OBJECT
  public:
    QGraphicsProxyWidget *m_startButton;
    StartView();
  public slots:
    void setButtonPos();
};

class ChooseLevelView : public FitGraphicsView {
    Q_OBJECT
  public:
    QGraphicsProxyWidget *chooseLevelDialog;
    ChooseLevelView();
  signals:
    void userStartLevel(qint32 gameLevel);

  public slots:
    void setWidgetPos();
};

class StartWindow : public QMainWindow {
    Q_OBJECT
  public:
    StartView *      m_startView;
    ChooseLevelView *m_chooseLevelView;
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    // bool eventFilter(QObject *object, QEvent *event);
  signals:
    void userStartGame();
  public slots:
    void changeToChooseLevelWindow();
};

#endif