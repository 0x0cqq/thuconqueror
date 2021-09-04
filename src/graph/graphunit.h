#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include "../basic/info.h"
#include "../basic/status.h"
#include <QDebug>
#include <QGraphicsObject>

class GraphUnit : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
  public:
    const UnitStatus *m_status;
    GraphUnit(UnitStatus *status, const QPointF &pos)
        : QGraphicsObject(), m_status(status) {
        this->setPos(pos);
        this->setFlag(ItemIsSelectable, true);
        qDebug() << "New unit " << m_status->m_uid << Qt::endl;
    }
    QRectF       boundingRect() const override;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  signals:
  public slots:
};

#endif  // GRAPHUNIT_H