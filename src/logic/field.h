#ifndef FIELD_H
#define FIELD_H

#include "../basic/status.h"
#include "block.h"
#include "unit.h"
#include <QObject>

class Field : public QObject {
    Q_OBJECT
  public:
    const GameInfo &          m_gameInfo;
    QVector<QVector<Block *>> m_blocks;
    QVector<Unit *>           m_units;

    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    bool   isValid(const QPoint &a) const {
        return a.x() >= 1 && a.x() <= width() && a.y() >= 1 &&
            a.y() <= height();
    }
    bool ableToPass(qint32 uid, QPoint p) {
        return ((blocks(p)->m_status->m_type & yesUnitBlock) != 0) &&
            ((blocks(p)->unitOnBlock() == -1 &&
              (((blocks(p)->m_status->m_type & campBlock) == 0) ||
               ((blocks(p)->m_status->m_type & campBlock) != 0 &&
                !notSameCamp(m_units[uid]->m_status, blocks(p)->m_status)))) ||
             (blocks(p)->unitOnBlock() != -1 &&
              m_units[blocks(p)->unitOnBlock()]->player() ==
                  m_units[uid]->player()));
    }
    QVector<QPoint> getPath(qint32 uid, QPoint start, QPoint end);
    Field(const GameInfo &                       gameInfo,
          const QVector<QVector<BlockStatus *>> &blockStatus,
          const QVector<UnitStatus *> &          unitStatus);
    ~Field();
    QVector<QPoint> __getUnitMoveRange(qint32 uid);
    QPoint          __canAttackNearby(qint32 uid);

  signals:
    void newUnit(UnitStatus *unitStatus);
    void unitDead(qint32 uid);
    void moveUnit(qint32 uid, QVector<QPoint> path);
    void attackUnit(qint32 uid, qint32 taruid);
    void attackCamp(qint32 uid, QPoint coord);
    void unitMoveRangegot(qint32 uid, QVector<QPoint> moveRange);
  protected slots:
    void doUnitMove(Unit *unit, QPoint coord);
    void doUnitAttack(Unit *unit, QPoint coord);
  public slots:
    void doNewUnit(UnitStatus *unitStatus);
    void doUnitMove(qint32 uid, QPoint coord);
    void doUnitAttack(qint32 uid, QPoint coord);
    void doUnitDie(qint32 uid);
    void getUnitMoveRange(qint32 uid);
};

#endif