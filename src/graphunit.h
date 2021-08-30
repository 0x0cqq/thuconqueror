#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "info.h"
#include <QGraphicsObject>
#include <QPainter>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
  public:
    GraphUnit() : QGraphicsObject() { this->setFlag(ItemIsSelectable, true); }
    GraphUnit(const QPointF &pos) : QGraphicsObject() {
        this->setPos(pos);
        this->setFlag(ItemIsSelectable, true);
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif  // GRAPHUNIT_H