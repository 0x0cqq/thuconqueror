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

void GraphUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    painter->setBrush(Qt::blue);
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
    // painter->drawRoundedRect(-100, -100, 200, 200, 50, 50);
    // painter->fillRect(0, 0, 100, 100, Qt::green);
}

void GraphUnit::mousePressEvent(QGraphicsSceneMouseEvent *event){
    static int i = 0;
    qDebug() << "press circle: " << this->pos() << Qt::endl;
    i++;
    QGraphicsObject::mousePressEvent(event);
    event->ignore();
}
