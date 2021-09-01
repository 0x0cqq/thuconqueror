#include "field.h"
#include <QDebug>

Field::Field(const GameInfo &                       gameInfo,
             const QVector<QVector<BlockStatus *>> &blockStatus,
             const QVector<UnitStatus *> &          unitStatus)
    : m_gameInfo(gameInfo) {
    m_blocks.resize(width() + 2);
    for(int i = 1; i <= width(); i++) {
        m_blocks[i].resize(height() + 2);
        for(int j = 1; j <= height(); j++) {
            m_blocks[i][j] = new Block(blockStatus[i][j]);
        }
    }
    for(int i = 0; i < unitStatus.size(); i++) {
    }
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

void Field::doUnitMove(Unit *unit, QPoint destCoord) {
    qDebug() << "move unit" << Qt::endl;
    emit moveUnit(unit->uid(), getPath(unit->nowCoord(), destCoord));
    blocks(unit->nowCoord())->unitOnBlock() = -1;
    unit->nowCoord()                        = destCoord;
    blocks(unit->nowCoord())->unitOnBlock() = unit->uid();
}
void Field::doUnitMove(qint32 uid, QPoint coord) {
    doUnitMove(m_units[uid], coord);
}

void Field::doUnitAttack(Unit *unit, QPoint coord) {
    Q_ASSERT(blocks(coord)->unitOnBlock() != -1);
    qint32 taruid = blocks(coord)->unitOnBlock();
    auto   att    = calculateAttack(unit->m_status, m_units[taruid]->m_status);
    bool   isAAlive = unit->m_status->changeHP(-att.first),
         isBAlive   = m_units[taruid]->m_status->changeHP(-att.second);
    if(!isAAlive)
        emit unitDead(unit->uid());
    if(!isBAlive)
        emit unitDead(taruid);
    emit attackUnit(unit->uid(), taruid);
}
void Field::doUnitAttack(qint32 uid, QPoint coord) {
    doUnitAttack(m_units[uid], coord);
}

void Field::doNewUnit(UnitStatus *unitStatus) {
    Q_ASSERT(unitStatus->m_uid == m_units.size());
    Unit *newunit = new Unit(unitStatus);
    m_units.append(newunit);
    blocks(newunit->nowCoord())->unitOnBlock() = newunit->uid();
    emit newUnit(unitStatus);
}

// void GraphField::moveUnit(GraphUnit *graphUnit, QPoint destCoord) {
//     this->moveUnit(graphUnit,
//                    getPath(graphUnit->m_status->m_nowCoord, destCoord));
//     return;
//     QPropertyAnimation *animation = new QPropertyAnimation(graphUnit, "pos");
//     animation->setDuration(1000);
//     animation->setStartValue(graphUnit->pos());
//     animation->setEndValue(getBlockCenter(destCoord));
//     // changeUnitCoord(graphUnit, destCoord);
//     animation->start();
// }
