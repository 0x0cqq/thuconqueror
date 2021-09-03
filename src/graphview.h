#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "graphfield.h"
#include <QDebug>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>

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
    GraphView()
        :  QGraphicsView(), m_scale(1.0), m_zoomDelta(0.1), m_translateButton(Qt::RightButton),
          m_translateSpeed(1.0), m_bMouseTranslate(false) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setCursor(Qt::PointingHandCursor);
        setRenderHint(QPainter::Antialiasing);

        // setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
        centerOn(0, 0);
    }
    void mouseMoveEvent(QMouseEvent *event) {
        if(m_bMouseTranslate) {
            QPointF mouseDelta =
                mapToScene(event->pos()) - mapToScene(m_lastMousePos);
            translate(mouseDelta);
        }

        m_lastMousePos = event->pos();

        QGraphicsView::mouseMoveEvent(event);
    }
    void resizeEvent(QResizeEvent *event) override {
        // if(this->scene() != nullptr) {
        //     this->fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);
        // }
        qDebug() << event->size();
        this->QGraphicsView::resizeEvent(event);
    }
    void setTranslateSpeed(qreal speed) {
        // 建议速度范围
        Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
                   "InteractiveView::setTranslateSpeed",
                   "Speed should be in range [0.0, 2.0].");
        m_translateSpeed = speed;
    }
    void mousePressEvent(QMouseEvent *event) {
        if(event->button() == m_translateButton) {
            // 当光标底下没有 item 时才能移动
            QPointF point = mapToScene(event->pos());
            // if(scene()->itemAt(point, transform()) == NULL) {
                qDebug() << "mouse move";
                m_bMouseTranslate = true;
                m_lastMousePos    = event->pos();
            // }
        }
        else{
            QGraphicsView::mousePressEvent(event);
        }
    }
    void mouseReleaseEvent(QMouseEvent *event) {
        if(event->button() == m_translateButton)
            m_bMouseTranslate = false;

        QGraphicsView::mouseReleaseEvent(event);
    }
    void translate(QPointF delta) {
        // 根据当前 zoom 缩放平移数
        delta *= m_scale;
        delta *= m_translateSpeed;

        // view 根据鼠标下的点作为锚点来定位 scene
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),
                         VIEW_HEIGHT / 2 - delta.y());
        centerOn(mapToScene(newCenter));

        // scene 在 view 的中心点作为锚点
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    }
    // 放大/缩小
    void wheelEvent(QWheelEvent *event) {
        // 滚轮的滚动量
        QPoint scrollAmount = event->angleDelta();
        // 正值表示滚轮远离使用者放大负值表示朝向使用者缩小
        scrollAmount.y() > 0 ? zoom(1 + m_zoomDelta) : zoom(1 - m_zoomDelta);
    }
    void setZoomDelta(qreal delta) {
        // 建议增量范围
        Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
                   "InteractiveView::setZoomDelta",
                   "Delta should be in range [0.0, 1.0].");
        m_zoomDelta = delta;
    }

    qreal zoomDelta() const { return m_zoomDelta; }
    void  zoom(float scaleFactor) {
        // 防止过小或过大
        qreal factor = transform()
                           .scale(scaleFactor, scaleFactor)
                           .mapRect(QRectF(0, 0, 1, 1))
                           .width();
        if(factor < 0.07 || factor > 100)
            return;

        scale(scaleFactor, scaleFactor);
        m_scale *= scaleFactor;
    }
};

#endif