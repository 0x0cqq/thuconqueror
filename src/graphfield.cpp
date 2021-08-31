#include "graphfield.h"
#include "graphblock.h"
#include <QPropertyAnimation>

GraphField::GraphField() : QGraphicsScene() {
    width = 24, height = 16;
    this->setSceneRect(
        QRectF(-1 * GraphInfo::blockSize, -qSqrt(3) / 2 * GraphInfo::blockSize,
               (1.5 * width + 0.5) * GraphInfo::blockSize,
               (qSqrt(3) * (height + 0.5)) * GraphInfo::blockSize));
    this->addRect(this->sceneRect());
    blocks.resize(width + 2);
    for(int i = 1; i <= width; i++) {
        blocks[i].resize(height + 2);
        for(int j = 1; j <= height; j++) {
            blocks[i][j] = new GraphBlock(QPoint{i, j}, getBlockCenter(i, j));
            this->addItem(blocks[i][j]);
        }
    }
    for(int i = 1; i <= width; i++) {
        for(int j = 1; j <= height; j++) {
            connect(blocks[i][j], &GraphBlock::checkChanged, this,
                    [this](QPoint coord, bool nowState) {
                        if(nowState == true) {
                            this->checkBlock(coord);
                        }
                        else {
                            this->unCheckBlock(coord);
                        }
                    });
        }
    }
}

QPointF GraphField::getBlockCenter(qint32 r, qint32 c) const {
    Q_ASSERT(1 <= r && r <= width);
    Q_ASSERT(1 <= c && c <= height);
    return QPointF(1.5 * (r - 1),
                   qSqrt(3) * (c - 1) + (r % 2 == 0 ? qSqrt(3) / 2 : 0)) *
        GraphInfo::blockSize;
}

QPointF GraphField::getBlockCenter(QPoint coord) const {
    return getBlockCenter(coord.x(), coord.y());
}

void GraphField::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "scene: " << event->scenePos() << Qt::endl;
    QGraphicsScene::mousePressEvent(event);
}

void GraphField::moveUnit(qint32 uid, const QVector<QPoint> &path) {
    Q_ASSERT(uid < units.size());
    this->moveUnit(units[uid], path);
}

// 不包括graphunit现在的点
void GraphField::moveUnit(GraphUnit *graphUnit, const QVector<QPoint> &path) {
    QPropertyAnimation *animation = new QPropertyAnimation(graphUnit, "pos");
    animation->setDuration(250 * path.size());
    animation->setStartValue(graphUnit->pos());
    animation->setEndValue(getBlockCenter(path.back()));
    qDebug() << "Path length:" << path.size() << Qt::endl;
    for(int i = 0; i < path.size() - 1; i++) {
        animation->setKeyValueAt(qreal(i + 1) / path.size(), getBlockCenter(path[i]));
    }
    animation->start();
    changeUnitCoord(graphUnit, path.back());
}

// 如果有其他的格子被选中了，且
void GraphField::checkBlock(QPoint coord) {
    if(m_nowCheckedBlock != nullptr) {
        if(m_nowCheckedBlock->m_unitOnBlock != nullptr &&
           blocks(coord)->m_unitOnBlock == nullptr) {
            GraphUnit *graphUnit = m_nowCheckedBlock->m_unitOnBlock;
            m_nowCheckedBlock->changeCheck(false);

            blocks(coord)->changeCheck(false);
            moveUnit(graphUnit, coord);
            return;
        }
        else {
            m_nowCheckedBlock->changeCheck(false);
        }
    }
    m_nowCheckedBlock = blocks(coord);
}

void GraphField::unCheckBlock(QPoint coord) {
    // Q_ASSERT(blocks(coord) == m_nowCheckedBlock);
    m_nowCheckedBlock = nullptr;
}

void GraphField::newUnit() {
    // 需要当前位置没有Unit，否则会炸掉的
    if(m_nowCheckedBlock == nullptr) {
        QMessageBox msgBox;
        msgBox.setText("没有选中Block!");
        int ret = msgBox.exec();
        return;
    }
    if(m_nowCheckedBlock->m_unitOnBlock != nullptr) {
        QMessageBox msgBox;
        msgBox.setText("当前Block已经有Unit了!");
        int ret = msgBox.exec();
        return;
    }

    GraphUnit *newUnit =
        new GraphUnit(units.size(), m_nowCheckedBlock->m_coord,
                      getBlockCenter(m_nowCheckedBlock->m_coord));
    units.append(newUnit);
    this->addItem(newUnit);
    setUnit(newUnit);
}

void GraphField::moveUnit(qint32 uid, QPoint destCoord) {
    Q_ASSERT(uid < units.size());
    moveUnit(units[uid], destCoord);
}


QVector<QPoint> getPath(QPoint start, QPoint end) {
    QVector<QPoint> ans;
    if(start.x() < end.x()) {
        for(int i = start.x() + 1; i <= end.x(); i++) {
            ans.append(QPoint(i, start.y()));
        }
    }
    else {
        for(int i = start.x() - 1; i >= end.x(); i--) {
            ans.append(QPoint(i, start.y()));
        }
    }
    if(start.y() < end.y()) {
        for(int i = start.y() + 1; i <= end.y(); i++) {
            ans.append(QPoint(end.x(), i));
        }
    }
    else {
        for(int i = start.y() - 1; i >= end.y(); i--) {
            ans.append(QPoint(end.x(), i));
        }
    }
    return ans;
}


void GraphField::moveUnit(GraphUnit *graphUnit, QPoint destCoord) {
    qDebug() << "move unit" << Qt::endl;
    this->moveUnit(graphUnit, getPath(graphUnit->m_nowCoord, destCoord));
    return;
    QPropertyAnimation *animation = new QPropertyAnimation(graphUnit, "pos");
    animation->setDuration(1000);
    animation->setStartValue(graphUnit->pos());
    animation->setEndValue(getBlockCenter(destCoord));
    changeUnitCoord(graphUnit, destCoord);
    animation->start();
}