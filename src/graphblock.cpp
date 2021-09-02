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
    if(m_isMoveRange && unitOnBlock() == -1) {
        painter->setBrush(Qt::blue);
    }
    else {
        if(!m_isChecked) {
            painter->setBrush(Qt::green);
        }
        else {
            painter->setBrush(Qt::red);
        }
    }
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawPolygon(GraphInfo::blockPoly);
    painter->setFont(QFont("Microsoft YaHei", 30, 2));
    painter->drawText(QPointF{-GraphInfo::blockSize / 2, 0},
                      m_status->m_type == plainBlock ? "" : "X");
}

void GraphBlock::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "block: " << coord().x() << coord().y() << Qt::endl;
    emit blockClicked(coord());
    // reverseCheck();
    QGraphicsObject::mousePressEvent(event);
}

void GraphBlock::changeCheck(QPoint coord, bool isChecked) {
    if(this->coord() != coord) {
        return;
    }
    m_isChecked = isChecked;
    emit this->checkChanged(coord(), m_isChecked);
}

void GraphBlock::changeMoveRange(QPoint coord, bool isMoveRange) {
    if(this->coord() != coord) {
        return;
    }
    m_isMoveRange = isMoveRange;
    emit this->moveRangeChanged(coord(), isMoveRange);
}
