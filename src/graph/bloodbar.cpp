#include "bloodbar.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

BloodBar::BloodBar(QGraphicsObject *parent)
    : QGraphicsObject(parent), percentange(1) {}

void BloodBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
    painter->setBrush(Qt::white);
    painter->drawRect(-GraphInfo::bloodWidth / 2, -GraphInfo::blockSize,
                      GraphInfo::bloodWidth, GraphInfo::bloodHeight);
    painter->setBrush(Qt::red);
    painter->drawRect(QRectF(-GraphInfo::bloodWidth / 2, -GraphInfo::blockSize,
                     percentange * GraphInfo::bloodWidth, GraphInfo::bloodHeight));
}

QRectF BloodBar::boundingRect() const {
    return QRectF(-GraphInfo::bloodWidth / 2, -GraphInfo::blockSize,
                  GraphInfo::bloodWidth, GraphInfo::bloodHeight);
}

void BloodBar::setPercentage(qreal _percentage) {
    percentange = _percentage;
    this->update(this->boundingRect());
}

void BloodBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsObject::mousePressEvent(event);
    event->ignore();
}