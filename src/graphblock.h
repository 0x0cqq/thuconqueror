#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "info.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QPainter>

class GraphBlock : public QGraphicsObject {
    Q_OBJECT
  public:
    QPoint m_coord;
    GraphBlock() : QGraphicsObject() {}
    GraphBlock(const QPoint coord, const QPointF &pos)
        : m_coord(coord), QGraphicsObject() {
        this->setPos(pos);
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        qDebug() << "block: " << m_coord.x() << m_coord.y() << Qt::endl;
        QGraphicsObject::mousePressEvent(event);
    }
};

#endif  // FIELDBLOCK_H