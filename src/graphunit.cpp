#include "graphunit.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

QRectF GraphUnit::boundingRect() const {
    return QRectF(-GraphInfo::unitSize - GraphInfo::penWidth,
                  -GraphInfo::unitSize - GraphInfo::penWidth,
                  2 * (GraphInfo::unitSize + GraphInfo::penWidth),
                  2 * (GraphInfo::unitSize + GraphInfo::penWidth));
}

QPainterPath GraphUnit::shape() const {
    QPainterPath path;
    path.addEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
    return path;
}

void GraphUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                      QWidget *) {
    if(m_status->isAlive()) {
        if(m_status->m_player == 1) {
            painter->setBrush(Qt::blue);
        }
        else {
            painter->setBrush(Qt::yellow);
        }
    }
    else {
        painter->setBrush(Qt::black);
    }
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
    // painter->drawRoundedRect(-100, -100, 200, 200, 50, 50);
    // painter->fillRect(0, 0, 100, 100, Qt::green);
}

void GraphUnit::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "press unit: " << this->m_status->m_uid << Qt::endl;
    QGraphicsObject::mousePressEvent(event);
    event->ignore();
}

void GraphUnit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "press unit: " << this->m_status->m_uid << Qt::endl;
    QGraphicsObject::mouseReleaseEvent(event);
    event->ignore();
}
