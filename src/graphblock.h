#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "info.h"
#include <QGraphicsObject>
#include <QPainter>

class GraphBlock : public QGraphicsObject {
    Q_OBJECT
  public:
    GraphBlock() : QGraphicsObject() {}
    GraphBlock(const QPointF &pos) : QGraphicsObject() { this->setPos(pos); }
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        
        QGraphicsObject::mousePressEvent(event);
    }
};

#endif  // FIELDBLOCK_H