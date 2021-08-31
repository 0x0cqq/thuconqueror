#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "info.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QPainter>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
  public:
    qint32 m_uid;
    QPoint m_nowCoord;
    GraphUnit() : QGraphicsObject() { this->setFlag(ItemIsSelectable, true); }
    GraphUnit(const qint32 uid, const QPoint &nowCoord, const QPointF &pos)
        : m_uid(uid), m_nowCoord(nowCoord), QGraphicsObject() {
        this->setPos(pos);
        this->setFlag(ItemIsSelectable, true);
        qDebug() << "New unit " << uid << Qt::endl;
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  signals:
  public slots:
};

#endif  // GRAPHUNIT_H