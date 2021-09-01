#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "graphunit.h"
#include "info.h"
#include "status.h"
#include <QDebug>
#include <QGraphicsObject>
#include <QPainter>

class GraphBlock : public QGraphicsObject {
    Q_OBJECT
  public:
    const BlockStatus *m_status;
    bool               m_isChecked;
    GraphBlock() : QGraphicsObject() {}
    GraphBlock(BlockStatus *status, QPointF pos)
        : QGraphicsObject(), m_status(status), m_isChecked(false) {
        this->setPos(pos);
        connect(this, &GraphBlock::checkChanged, this,
                [&]() { this->update(this->boundingRect()); });
    }
    // QPoint coord() const {return m_status->m_coord;}
    // qint32 unitOnBlock() { return m_status->m_unitOnBlock;}
    QRectF       boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
  signals:
    void blockClicked(QPoint coord);
    void checkChanged(QPoint coord, bool nowState);
  public slots:
    void changeCheck(QPoint coord, bool isChecked);
};

#endif  // FIELDBLOCK_H