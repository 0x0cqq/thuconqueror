#ifndef GRAPHFIELD_H
#define GRAPHFIELD_H

#include "info.h"
#include "graphblock.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


class GraphField : public QGraphicsScene {
    Q_OBJECT
  public:
    qint32 width, height;
    GraphField();
    QPointF getBlockCenter(qint32 r, qint32 c) const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif