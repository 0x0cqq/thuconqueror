#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "graphfield.h"
#include <QDebug>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

class GraphView : public QGraphicsView {
    Q_OBJECT
  public:
    qreal           m_scale;            // 缩放值
    qreal           m_translateSpeed;   // 平移速度
    qreal           m_zoomDelta;        // 缩放值变化量
    Qt::MouseButton m_translateButton;  // 平移所用的键
    bool            m_bMouseTranslate;
    QPoint          m_lastMousePos;
  signals:
    void finishPainting();

  public:
    GraphView()
        : QGraphicsView(), m_scale(1.0), m_zoomDelta(0.1),
          m_translateButton(Qt::RightButton), m_translateSpeed(1.0),
          m_bMouseTranslate(false) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setCursor(Qt::PointingHandCursor);
        setRenderHint(QPainter::Antialiasing);

        // setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
        centerOn(0, 0);
    }
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void setTranslateSpeed(qreal speed);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void translate(QPointF delta);
    // 放大/缩小
    void wheelEvent(QWheelEvent *event);
    void setZoomDelta(qreal delta);

    qreal zoomDelta() const;
    void  zoom(float scaleFactor);
};

#endif