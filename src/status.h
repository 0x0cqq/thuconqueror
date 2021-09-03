#ifndef STATUS_H
#define STATUS_H

#include "info.h"
#include <QObject>
#include <QPoint>
#include <QtGlobal>

enum BlockType {
    yesUnitBlock    = 1,  // 可以去，而不是现在有没有
    noUnitBlock     = 1 << 1,
    plainBlock      = yesUnitBlock | 1 << 2,
    obstacleBlock   = noUnitBlock | 1 << 3,
    dampBlock       = yesUnitBlock | 1 << 4,
    virusCampBlock  = yesUnitBlock | 1 << 5,
    peopleCampBlock = yesUnitBlock | 1 << 6
};

#define blocks(point) m_blocks[(point).x()][(point).y()]
#define uid()         m_status->m_uid
#define unitOnBlock() m_status->m_unitOnBlock
#define player()      m_status->m_player
#define coord()       m_status->m_coord
#define nowCoord()    m_status->m_nowCoord

class BlockStatus : public QObject {
    Q_OBJECT
  public:
    const BlockType m_type;
    const QPoint    m_coord;
    qint32          m_unitOnBlock;
    qint32          MPneed() const { return 1; }
    BlockStatus(const BlockType &type, QPoint coord)
        : m_type(type), m_coord(coord), m_unitOnBlock(-1) {}
};

class UnitStatus : public QObject {
    Q_OBJECT
  public:
    // return true if HP < 0
    const qint32    m_uid;
    const UnitInfo &m_info;
    const UnitType  m_type;
    qint32          m_player;
    QPoint          m_nowCoord;
    qreal           m_HPnow;  // [0,1]
    qreal getHP() const { return m_info.HPfull * m_info.HPratio * m_HPnow; }
    qreal getCE() const { return m_info.CEfull * m_info.CEratio * m_HPnow; }
    qreal getMP() const { return m_info.MPfull; }
    bool  changeHP(qreal delta);
    bool  isAlive() const { return this->getHP() > 0; }
    UnitStatus(const int &uid, const UnitType type, const UnitInfo &uInfo,
               qint32 player, QPoint coord);
};

// calculate the attack point (source, target)
QPair<qreal, qreal> calculateAttack(UnitStatus *source, UnitStatus *target);



bool isNearByPoint(const QPoint &a, const QPoint &b);

bool isNearbyBlock(const BlockStatus *a, const BlockStatus *b);

QVector<QPoint> getNearbyPoint(const QPoint &a);

#endif
