#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H


#include "graphinfo.h"
#include <QGraphicsObject>
#include <QPainter>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
  public:
    QRectF boundingRect() const override;
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) override;

    
};

#endif  // GRAPHUNIT_H