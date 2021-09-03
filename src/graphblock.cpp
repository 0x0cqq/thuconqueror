#include "graphblock.h"
#include "graphfield.h"
#include <QGraphicsSceneMouseEvent>

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
    painter->setFont(QFont("Microsoft YaHei", 30, 2));
    painter->drawText(QPointF{-GraphInfo::blockSize / 2, 0},
                      m_status->m_type == plainBlock ? "" : "X");
    auto s = static_cast<GraphField *>(scene());
    if(m_isMoveRange) {
        if(unitOnBlock() == -1) {
            QColor blue40 = Qt::blue;
            blue40.setAlphaF(0.4);
            painter->setBrush(blue40);
            // painter->setPen(QPen(Qt::black, 0));

            painter->drawPolygon(GraphInfo::blockPoly);
        }
    }
    if(unitOnBlock() != -1 && s->m_nowCheckedBlock != nullptr &&
       s->m_nowCheckedBlock->unitOnBlock() != -1) {
        if(isNearByPoint(s->m_nowCheckedBlock->coord(), coord()) &&
           s->units[unitOnBlock()]->player() !=
               s->units[s->m_nowCheckedBlock->unitOnBlock()]->player() &&
           s->units[s->m_nowCheckedBlock->unitOnBlock()]->player() ==
               s->m_gameInfo.nowPlayer) {
            // 可以攻击
            QColor red40 = Qt::red;
            red40.setAlphaF(0.4);
            painter->setBrush(red40);
            painter->drawPolygon(GraphInfo::blockPoly);
        }
    }
}

void GraphBlock::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        qDebug() << "press block: " << coord().x() << coord().y() << Qt::endl;
        // emit blockClicked(coord());
        // reverseCheck();
        QGraphicsObject::mousePressEvent(event);
        event->accept();
    }
}

void GraphBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        qDebug() << "release block: " << coord().x() << coord().y() << Qt::endl;
        emit blockClicked(coord());
        // reverseCheck();
        QGraphicsObject::mouseReleaseEvent(event);
    }
}

void GraphBlock::changeCheck(QPoint coord, bool isChecked) {
    if(this->coord() != coord) {
        return;
    }
    m_isChecked = isChecked;
    emit this->checkChanged(coord(), m_isChecked);
}

void GraphBlock::changeMoveRange(qint32 uid, QPoint coord, bool isMoveRange) {
    if(this->coord() != coord) {
        return;
    }
    m_isMoveRange = isMoveRange;
    emit this->moveRangeChanged(coord(), isMoveRange);
}
