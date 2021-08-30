#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H


#include "graphinfo.h"
#include <QPainter>
#include <QGraphicsObject>

class GraphBlock : public QGraphicsObject {
    Q_OBJECT
    QRectF boundingRect() const override;
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) override;
};

#endif  // FIELDBLOCK_H