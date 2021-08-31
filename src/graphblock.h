#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "graphunit.h"
#include "info.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QPainter>

class GraphBlock : public QGraphicsObject {
    Q_OBJECT
  public:
    QPoint     m_coord;
    bool       m_isChecked;
    GraphUnit *m_unitOnBlock;
    GraphBlock() : QGraphicsObject() {}
    GraphBlock(const QPoint coord, const QPointF &pos)
        : QGraphicsObject(), m_coord(coord), m_isChecked(false),m_unitOnBlock(nullptr) {
        this->setPos(pos);
        connect(this, &GraphBlock::checkChanged, this,
                [&]() { this->update(this->boundingRect()); });
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
  signals:
    void checkChanged(QPoint coord, bool nowState);
  public slots:
    void changeCheck(bool isChecked);
    void reverseCheck();
};

#endif  // FIELDBLOCK_H