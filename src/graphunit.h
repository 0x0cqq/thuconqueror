#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "info.h"
#include <QGraphicsObject>
#include <QPainter>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
  public:
    qint32 uid;
    QPoint m_nowCoord;
    GraphUnit() : QGraphicsObject() { this->setFlag(ItemIsSelectable, true); }
    GraphUnit(const QPoint &nowCoord, const QPointF &pos)
        : m_nowCoord(nowCoord), QGraphicsObject() {
        this->setPos(pos);
        this->setFlag(ItemIsSelectable, true);
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  signals:
    void checked(qint32 uid);
    void unChecked(qint32 uid);

  public slots:
};

#endif  // GRAPHUNIT_H