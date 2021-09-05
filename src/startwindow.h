#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPushButton>

class FitGraphicsView : public QGraphicsView {
    Q_OBJECT

  public:
    QGraphicsScene *m_scene;

    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    FitGraphicsView();
    ~FitGraphicsView();
  signals:
    void finishPainting();
};

class StartView : public FitGraphicsView {
    Q_OBJECT
  public:
    QGraphicsPixmapItem * m_background;
    QGraphicsProxyWidget *m_startButton;
    StartView();
  public slots:
    void setButtonPos();
};

class ChooseLevelView : public FitGraphicsView {
    Q_OBJECT
    QGraphicsPixmapItem *m_background;

  public:
    ChooseLevelView();
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