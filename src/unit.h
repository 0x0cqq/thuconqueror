#ifndef UNIT_H
#define UNIT_H

#include "graphunit.h"
#include <QObject>

class UnitInfo {
  public:
    // HP == Health Point
    qint32 HPfull;
    qreal  HPratio;
    // Combat Effectiveness
    qint32 CEfull;
    qreal  CEratio;
    // movePoint
    qint32 MPfull;

    UnitInfo();
    UnitInfo(qint32, qint32, qint32);
};

class UnitStatus {
    // return true if HP < 0

    const UnitInfo &uInfo;
    qreal           HPnow;  // [0,1]
  public:
    qreal getHP() const { return uInfo.HPfull * uInfo.HPratio * HPnow; }
    qreal getCE() const { return uInfo.CEfull * uInfo.CEratio * HPnow; }
    qreal getMP() const { return uInfo.HPfull; }
    bool  changeHP(qreal delta);
    UnitStatus(const UnitInfo &);
};

enum UnitType {
    virusUnit   = 1,
    peopleUnit  = 1 << 1,
    studentUnit = peopleUnit | 1 << 2,
    teacherUnit = peopleUnit | 1 << 3,
    childUnit   = peopleUnit | 1 << 4
    // setting: 第一位和第二位是判断类型的，
};

class Block;

class Unit : public QObject {
    Q_OBJECT
  public:
    UnitStatus unitStatus;
    UnitType   uType;
    bool       isVirusUnit() const { return uType & virusUnit; }
    bool       isPeopleUnit() const { return uType & peopleUnit; }
    bool       isSameType(Unit *target) const { return uType & target->uType; }
    void       attack(Unit *enemyUnit);
    Block *    nowBlock;
    QPair<qreal, qreal> calculateAttack(Unit *target);
    qint32              uid;
    bool                isAlive() const;
  signals:
    void unitDead();
  public slots:
    
};

#endif