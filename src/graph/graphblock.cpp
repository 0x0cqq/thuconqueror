#include "graphblock.h"
#include "graphfield.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QPainterPath>

QRectF GraphBlock::boundingRect() const {
    return  GraphInfo::blockPoly.boundingRect();
    return QRectF(-qSqrt(3) / 2 * GraphInfo::blockSize - GraphInfo::penWidth,
                  -GraphInfo::blockSize - GraphInfo::penWidth,
                  qSqrt(3) * GraphInfo::blockSize + 2 * GraphInfo::penWidth,
                  2 * GraphInfo::blockSize + 2 * GraphInfo::penWidth);
}

QPainterPath GraphBlock::shape() const {
    QPainterPath path;
    path.addPolygon(GraphInfo::blockPoly);
    return path;
}

void GraphBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                       QWidget *) {
    painter->setPen(QPen(Qt::black, GraphInfo::penWidth));
    painter->drawPixmap(boundingRect(), *(m_status->m_info->pixmap),
                        m_status->m_info->pixmap->rect());
    painter->drawPolygon(GraphInfo::blockPoly);
    if(m_isChecked) {
        this->setZValue(GraphInfo::blockZValue + 1);
        painter->setPen(QPen(Qt::red, GraphInfo::penWidth));
        painter->drawRect(GraphInfo::blockPoly.boundingRect());
    }
    else{
        this->setZValue(GraphInfo::blockZValue);
    }
    // if(m_status->m_type == roadBlock) {
    //     painter->setFont(QFont("Microsoft YaHei", 10, 2));
    // }
    // else if(m_status->m_type == obstacleBlock) {
    //     painter->setFont(QFont("Microsoft YaHei", 40, 15));
    // }
    // else {
    //     painter->setFont(QFont("Microsoft YaHei", 20, 1));
    // }
    // if(m_status->m_type != plainBlock)
    //     painter->drawText(QPointF{-GraphInfo::blockSize / 2, 0},
    //                       QString::number(m_status->m_type));
    auto s = static_cast<GraphField *>(scene());
    if(m_isMoveRange) {
        if(unitOnBlock() == -1) {
            QColor color40;
            if(s->m_units[s->m_nowCheckedBlock->unitOnBlock()]->player() == 1) {
                color40 = Qt::blue;
            }
            else {
                color40 = Qt::yellow;
            }
            color40.setAlphaF(0.4);
            painter->setBrush(color40);
            // painter->setPen(QPen(Qt::black, 0));

            painter->drawPolygon(GraphInfo::blockPoly);
        }
    }
    bool canBeAttacked = false;
    if(unitOnBlock() != -1 && s->m_nowCheckedBlock != nullptr &&
       s->m_nowCheckedBlock->unitOnBlock() != -1) {
        GraphUnit *a = s->m_units[s->m_nowCheckedBlock->unitOnBlock()],
                  *b = s->m_units[unitOnBlock()];
        if(a->player() == s->m_gameInfo.nowPlayer &&
           canUnitAttack(a->m_status, b->m_status)) {
            // 可以攻击
            canBeAttacked = true;
        }
    }
    if(s->m_nowCheckedBlock != nullptr &&
       s->m_nowCheckedBlock->unitOnBlock() != -1) {
        GraphUnit *a = s->m_units[s->m_nowCheckedBlock->unitOnBlock()];
        if(a->player() == s->m_gameInfo.nowPlayer &&
           canUnitAttackBlock(a->m_status, m_status)) {
            // 可以攻击
            canBeAttacked = true;
        }
    }
    if(canBeAttacked) {
        QColor red40 = Qt::red;
        red40.setAlphaF(0.4);
        painter->setBrush(red40);
        painter->drawPolygon(GraphInfo::blockPoly);
    }
    if(m_blockCampBlood){
        m_blockCampBlood->setPercentage(m_status->m_HPnow);// 其实不应该在这里判断逻辑，不过差不多得了
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
        // 就用这个函数？
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
