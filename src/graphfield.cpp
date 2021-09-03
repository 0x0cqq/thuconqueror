#include "graphfield.h"
#include "graphblock.h"
#include <QGraphicsView>
#include <QPropertyAnimation>

GraphField::GraphField(const GameInfo &                 gameInfo,
                       QVector<QVector<BlockStatus *>> &blockStatus,
                       QVector<UnitStatus *> &          unitStatus)
    : QGraphicsScene(), m_gameInfo(gameInfo), m_nowCheckedBlock(nullptr) {
    this->setSceneRect(
        QRectF(-1 * GraphInfo::blockSize, -qSqrt(3) / 2 * GraphInfo::blockSize,
               (1.5 * width() + 0.5) * GraphInfo::blockSize,
               (qSqrt(3) * (height() + 0.5)) * GraphInfo::blockSize));
    this->addRect(this->sceneRect());
    m_blocks.resize(width() + 2);
    for(int i = 1; i <= width(); i++) {
        m_blocks[i].resize(height() + 2);
        for(int j = 1; j <= height(); j++) {
            m_blocks[i][j] =
                new GraphBlock(blockStatus[i][j], getBlockCenter(i, j));
            this->addItem(m_blocks[i][j]);
        }
    }
    for(int i = 1; i <= width(); i++) {
        for(int j = 1; j <= height(); j++) {
            connect(m_blocks[i][j], &GraphBlock::blockClicked, this,
                    &GraphField::onBlockClicked);

            connect(this, &GraphField::checkStateChange, m_blocks[i][j],
                    &GraphBlock::changeCheck);
            connect(this, &GraphField::moveRangeChange, m_blocks[i][j],
                    &GraphBlock::changeMoveRange);
        }
    }
    connect(this, &GraphField::checkStateChange, this,
            [=](QPoint coord, bool state) {
                qint32 uid = blocks(coord)->unitOnBlock();
                if(state == true && uid != -1 &&
                   units[uid]->player() == m_gameInfo.nowPlayer &&
                   units[uid]->m_status->isAlive()) {
                    emit userShowMoveRange(uid);
                }
                else {
                    emit userHideMoveRange();
                }
            });

}

QPointF GraphField::getBlockCenter(qint32 r, qint32 c) const {
    Q_ASSERT(1 <= r && r <= width());
    Q_ASSERT(1 <= c && c <= height());
    return QPointF(1.5 * (r - 1),
                   qSqrt(3) * (c - 1) + (r % 2 == 0 ? qSqrt(3) / 2 : 0)) *
        GraphInfo::blockSize;
}

QPointF GraphField::getBlockCenter(QPoint coord) const {
    return getBlockCenter(coord.x(), coord.y());
}

void GraphField::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "scene: " << event->scenePos() << Qt::endl;

    qDebug() << "view: " << views().first()->mapFromScene(event->scenePos())
             << Qt::endl;
    qDebug() << "view: "
             << views().first()->mapToScene(
                    views().first()->mapFromScene(event->scenePos()))
             << Qt::endl;
    QGraphicsScene::mousePressEvent(event);
}

void GraphField::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "release scene: " << event->scenePos() << Qt::endl;
    QGraphicsScene::mouseReleaseEvent(event);
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
        animation->setKeyValueAt(qreal(i + 1) / path.size(),
                                 getBlockCenter(path[i]));
    }
    animation->start();
    emit needUpdateDetail();
}

void GraphField::onBlockClicked(QPoint coord) {
    if(m_nowCheckedBlock == nullptr) {
        m_nowCheckedBlock = blocks(coord);
        emit checkStateChange(coord, true);
    }
    else {
        if(m_nowCheckedBlock->coord() == coord) {
            m_nowCheckedBlock = nullptr;
            emit checkStateChange(coord, false);
        }
        else {
            qint32 flag = -1;
            // flag == 0: 更换选择格
            // flag == 1: 移动
            // flag == 2: 攻击
            qint32 uidA = m_nowCheckedBlock->unitOnBlock(),
                   uidB = blocks(coord)->unitOnBlock();
            if(uidA != -1) {
                // A 格子上有棋子
                if(m_gameInfo.nowPlayer == units[uidA]->m_status->m_player &&
                   units[uidA]->m_status->isAlive()) {
                    // A 格上是当前玩家的棋子 且没有死
                    if(uidB == -1) {
                        // B 格子上没有棋子
                        if(blocks(coord)->m_isMoveRange) {
                            flag = 1;
                        }
                        else {
                            flag = 0;
                        }
                    }
                    else {
                        // B 格子上有棋子
                        if(m_gameInfo.nowPlayer ==
                           units[uidB]->m_status->m_player) {
                            // B 格上是当前玩家的棋子
                            flag = 0;
                        }
                        else {
                            // B 格上不是当前玩家的棋子
                            if(units[uidB]->m_status->isAlive()) {
                                // 活着
                                flag = 2;
                            }
                            else {
                                // 死了
                                flag = 0;
                            }
                        }
                    }
                }
                else {
                    // A 格上不是当前玩家的棋子
                    flag = 0;
                }
            }
            else {
                flag = 0;
            }
            GraphBlock *tmp_block = m_nowCheckedBlock;
            switch(flag) {
                case 0:
                    emit checkStateChange(tmp_block->coord(), false);
                    m_nowCheckedBlock = nullptr;

                    m_nowCheckedBlock = blocks(coord);
                    emit checkStateChange(coord, true);
                    break;
                case 1:

                    emit checkStateChange(tmp_block->coord(), false);
                    m_nowCheckedBlock = nullptr;
                    emit userMoveUnit(uidA, coord);
                    break;
                case 2:

                    emit checkStateChange(tmp_block->coord(), false);
                    m_nowCheckedBlock = nullptr;
                    emit userAttackUnit(uidA, coord);
                    break;
                default:
                    break;
            }
        }
    }
}

void GraphField::newUnit(UnitStatus *unitStatus) {
    qDebug() << "new unit: " << unitStatus->m_nowCoord << Qt::endl;
    GraphUnit *newUnit =
        new GraphUnit(unitStatus, getBlockCenter(unitStatus->m_nowCoord));
    units.append(newUnit);
    this->addItem(newUnit);
    emit needUpdateDetail();
}

void GraphField::attackUnit(qint32 uid, qint32 tarid) {
    attackUnit(units[uid], tarid);
}
void GraphField::attackUnit(GraphUnit *graphUnit, qint32 tarid) {
    graphUnit->update(graphUnit->boundingRect());
    units[tarid]->update(units[tarid]->boundingRect());
    QMessageBox msgBox;
    msgBox.setText("攻击！ " + QString::number(graphUnit->uid()) +
                   " 号 Unit 攻击了 " + QString::number(tarid) + " 号 Unit 。");
    msgBox.exec();
    emit needUpdateDetail();
}

void GraphField::dieUnit(qint32 uid) {
    units[uid]->update(units[uid]->boundingRect());
    emit needUpdateDetail();
}

void GraphField::showMoveRange(QVector<QPoint> range) {
    for(auto p : range) {
        emit moveRangeChange(p, true);
    }
}

void GraphField::hideMoveRange() {
    for(int i = 1; i <= width(); i++) {
        for(int j = 1; j <= height(); j++) {
            emit moveRangeChange(QPoint(i, j), false);
        }
    }
}
