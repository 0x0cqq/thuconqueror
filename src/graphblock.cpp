#include "graphblock.h"

QRectF GraphBlock::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(
        -GraphInfo::blockSize - penWidth, -GraphInfo::blockSize - penWidth,
        GraphInfo::blockSize + penWidth, GraphInfo::blockSize + penWidth);
}

void GraphBlock::paint(QPainter *                      painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *                       widget) {
    painter->setBrush(Qt::red);
    painter->setPen(Qt::black);
    painter->drawPolygon(GraphInfo::blockPoly);
}