#include "graphview.h"

void GraphView::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
    emit finishPainting();
}
void GraphView::mouseMoveEvent(QMouseEvent *event) {
    if(m_bMouseTranslate) {
        QPointF mouseDelta =
            mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        translate(mouseDelta);
    }
    m_lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}
void GraphView::resizeEvent(QResizeEvent *event) {
    qDebug() << event->size();
    this->QGraphicsView::resizeEvent(event);
}
void GraphView::setTranslateSpeed(qreal speed) {
    // 建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "InteractiveView::setTranslateSpeed",
               "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}
void GraphView::mousePressEvent(QMouseEvent *event) {
    if(event->button() == m_translateButton) {
        qDebug() << "mouse move";
        m_bMouseTranslate = true;
        m_lastMousePos    = event->pos();
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}
void GraphView::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == m_translateButton)
        m_bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}
void GraphView::translate(QPointF delta) {
    // 根据当前 zoom 缩放平移数
    delta *= m_scale;
    delta *= m_translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
// 放大/缩小
void GraphView::wheelEvent(QWheelEvent *event) {
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者放大负值表示朝向使用者缩小
    scrollAmount.y() > 0 ? zoom(1 + m_zoomDelta) : zoom(1 - m_zoomDelta);
}
void GraphView::setZoomDelta(qreal delta) {
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0, "InteractiveView::setZoomDelta",
               "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

qreal GraphView::zoomDelta() const {
    return m_zoomDelta;
}
void GraphView::zoom(float scaleFactor) {
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
