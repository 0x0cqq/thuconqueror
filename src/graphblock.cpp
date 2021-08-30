#include "graphblock.h"

QRectF GraphBlock::boundingRect() const {
    return QRectF(-GraphInfo::blockSize - GraphInfo::penWidth,
                  -GraphInfo::blockSize - GraphInfo::penWidth,
                  2 * (GraphInfo::blockSize + GraphInfo::penWidth),
                  2 * (GraphInfo::blockSize + GraphInfo::penWidth));
}

QPainterPath GraphBlock::shape() const {
    QPainterPath path;
    path.addPolygon(GraphInfo::blockPoly);
    return path;
}

void GraphBlock::paint(QPainter *                      painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *                       widget) {
    painter->setBrush(Qt::red);
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawPolygon(GraphInfo::blockPoly);
}