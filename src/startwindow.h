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

  protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
  signals:
    void finishPainting();
};

class StartWindow : public QMainWindow {
    Q_OBJECT
  public:
    FitGraphicsView *    m_view;
    QGraphicsScene *     m_scene;
    QGraphicsPixmapItem *m_background;
    QGraphicsProxyWidget *m_startButton;

    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    void setButtonPos();
    // bool eventFilter(QObject *object, QEvent *event);
  signals:
    void userStartGame();
};

#endif