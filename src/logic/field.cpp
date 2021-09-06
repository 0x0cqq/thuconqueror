#include "field.h"
#include <QDebug>
#include <QMap>
#include <algorithm>
#include <set>

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
        m_units.push_back(new Unit(unitStatus[i]));
    }
}

Field::~Field() {
    for(int i = 1; i <= width(); i++) {
        for(int j = 1; j <= height(); j++) {
            delete m_blocks[i][j];
        }
    }
    for(int i = 0; i < m_units.size(); i++) {
        delete m_units[i];
    }
}

bool cmp(const QPoint &a, const QPoint &b) {
    if(a.x() < b.x()) {
        return a.x() < b.x();
    }
    else {
        return a.y() < b.y();
    }
}

struct distance {
    qreal  dist;
    QPoint coord;
    bool   operator<(const distance &b) const {
        if(dist != b.dist)
            return dist > b.dist;
        else {
            return cmp(coord, b.coord);
        }
    }
};

bool operator<(const QPoint &a, const QPoint &b) {
    if(a.x() != b.x()) {
        return a.x() < b.x();
    }
    else {
        return a.y() < b.y();
    }
}

QVector<QPoint> Field::getPath(qint32 uid, QPoint start, QPoint end) {
    QVector<QPoint> ans;
    // distance, coord, last coord;
    qDebug() << "get path" << Qt::endl;
    QMap<distance, QPoint> q;
    QMap<QPoint, qreal>    dis;
    QMap<QPoint, QPoint>   from;
    QMap<QPoint, bool>     vis;
    q.insert(distance{0, start}, QPoint{0, 0});

    // vis[qMakePair(tmpCoord.x(),tmpCoord.y())] = true;
    while(!q.empty()) {
        qreal  dist = q.begin().key().dist;
        QPoint pos  = q.begin().key().coord;
        // QPoint fr   = q.begin().value();
        q.erase(q.begin());
        if(pos == end) {
            break;
        }
        if(vis.contains(pos))
            continue;
        vis.insert(pos, 1);
        // qDebug() << "pos: "<< pos << Qt::endl;

        QVector<QPoint> nearby = getNearbyPoint(pos);
        for(auto p : nearby)
            if(isValid(p)) {
                qreal newdist = dist - blocks(p)->m_status->MPneed();
                if((!dis.contains(p) || newdist > dis[p]) &&
                   ableToPass(uid, p)) {
                    // qDebug() << p << dis.contains(p) << Qt::endl;
                    dis[p] = dist;
                    q.insert(distance{newdist, p}, pos);
                    from[p] = pos;
                }
            }
    }
    // qDebug() << dis[end];
    QPoint tmp = end;
    while(tmp != start) {
        // if(tmp == QPoint(0, 0)) {
        //     break;
        // }
        ans.push_back(tmp);
        // qDebug() << tmp << Qt::endl;
        tmp = from[tmp];
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

void Field::doUnitMove(Unit *unit, QPoint destCoord) {
    qDebug() << "move unit" << Qt::endl;
    emit moveUnit(unit->uid(),
                  getPath(unit->uid(), unit->nowCoord(), destCoord));
    blocks(unit->nowCoord())->unitOnBlock() = -1;
    unit->nowCoord()                        = destCoord;
    blocks(unit->nowCoord())->unitOnBlock() = unit->uid();
    unit->m_status->setMoveState(false);
    auto np = getNearbyPoint(unit->nowCoord());
    bool flag = false;  // 检测是否旁边一个可以攻击的敌方元素都没有
    for(auto nearPoint : np) {
        if(blocks(nearPoint)->unitOnBlock() == -1)
            continue;
        if(canUnitAttack(unit->m_status,
                         m_units[blocks(nearPoint)->unitOnBlock()]->m_status)) {
            flag = true;
            break;
        }
    }
    unit->m_status->setAttackState(flag);
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
    unit->m_status->setAttackState(false);
    unit->m_status->setMoveState(false);
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

void Field::getUnitMoveRange(qint32 uid) {
    // distance, coord, last coord;
    qDebug() << "calculate move range" << Qt::endl;
    QVector<QPoint>        moveRange;
    QMap<distance, QPoint> q;
    QMap<QPoint, qreal>    dis;
    QMap<QPoint, bool>     vis;
    QPoint                 tmpCoord = m_units[uid]->nowCoord();
    dis[tmpCoord]                   = m_units[uid]->m_status->getMP();
    q.insert(distance{m_units[uid]->m_status->getMP(), tmpCoord}, QPoint{0, 0});

    // vis[qMakePair(tmpCoord.x(),tmpCoord.y())] = true;
    while(!q.empty()) {
        qreal  dist = q.begin().key().dist;
        QPoint pos  = q.begin().key().coord;
        // QPoint from = q.begin().value();
        q.erase(q.begin());
        if(vis.contains(pos))
            continue;
        vis.insert(pos, 1);

        if(dist < 0)
            break;

        moveRange.push_back(pos);
        QVector<QPoint> nearby = getNearbyPoint(pos);
        for(auto p : nearby) {
            // qDebug() << "\t" << p << Qt::endl;

            if(isValid(p)) {
                qreal newdist = dist - blocks(p)->m_status->MPneed();
                if((!dis.contains(p) || newdist > dis[p]) &&
                   ableToPass(uid, p)) {
                    dis[p] = newdist;
                    q.insert(distance{newdist, p}, pos);
                }
            }
        }
    }
    moveRange.erase(moveRange.begin());  // 抹掉本身
    emit unitMoveRangegot(uid, moveRange);
    qDebug() << moveRange.size() << Qt::endl;
}
