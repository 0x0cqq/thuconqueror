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
    if(!m_isChecked) {
        painter->setBrush(Qt::green);
    }
    else {
        painter->setBrush(Qt::red);
    }
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawPolygon(GraphInfo::blockPoly);
}

void GraphBlock::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "block: " << m_status->m_coord.x() << m_status->m_coord.y()
             << Qt::endl;
    emit blockClicked(m_status->m_coord);
    // reverseCheck();
    QGraphicsObject::mousePressEvent(event);
}

void GraphBlock::changeCheck(QPoint coord, bool isChecked) {
    if(this->m_status->m_coord != coord) {
        return;
    }
    m_isChecked = isChecked;
    emit this->checkChanged(m_status->m_coord, m_isChecked);
}