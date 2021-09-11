#include "bloodbar.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

BloodBar::BloodBar(qreal p, qreal w, qreal h, QGraphicsObject *parent)
    : QGraphicsObject(parent), m_bloodWidth(w), m_bloodHeight(h), m_pos(p),
      percentange(1) {}

void BloodBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
    painter->setBrush(Qt::white);
    painter->drawRect(-m_bloodWidth / 2, m_pos, m_bloodWidth, m_bloodHeight);
    painter->setBrush(Qt::red);
    if(percentange > 0) {
        painter->drawRect(QRectF(-m_bloodWidth / 2, m_pos,
                                 percentange * m_bloodWidth, m_bloodHeight));
    }
}

QRectF BloodBar::boundingRect() const {
    return QRectF(-m_bloodWidth / 2, m_pos, m_bloodWidth, m_bloodHeight);
}

void BloodBar::setPercentage(qreal _percentage) {
    percentange = _percentage;
    this->update(this->boundingRect());
}

void BloodBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mousePressEvent(event);
    event->ignore();
}