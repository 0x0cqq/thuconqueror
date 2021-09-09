// 说是 blood bar，但我更想把其设置成一个圆盘的样子xs

#ifndef BLOODBAR_H
#define BLOODBAR_H

#include "../basic/status.h"
#include <QGraphicsObject>

class BloodBar : public QGraphicsObject {
    Q_OBJECT
    qreal m_bloodWidth, m_bloodHeight, m_pos;
    qreal percentange;

  public:
    // p 是在上下什么位置放，Camp的就在底下，Unit 就在上面
    BloodBar(qreal p, qreal w, qreal h, QGraphicsObject *parent);
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) override;
    // 设置血条的百分比
    void   setPercentage(qreal percentage);
    QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif