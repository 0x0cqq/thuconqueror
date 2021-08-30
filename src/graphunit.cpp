#include "graphunit.h"

QRectF GraphUnit::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(
        -GraphInfo::unitSize - penWidth, -GraphInfo::unitSize - penWidth,
        GraphInfo::unitSize + penWidth, GraphInfo::unitSize + penWidth);
}

void GraphUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    painter->setBrush(Qt::blue);
    painter->drawEllipse({0, 0}, GraphInfo::unitSize, GraphInfo::unitSize);
    // painter->drawRoundedRect(-100, -100, 200, 200, 50, 50);
    // painter->fillRect(0, 0, 100, 100, Qt::green);
}