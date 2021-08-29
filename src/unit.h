#ifndef UNIT_H
#define UNIT_H

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

    UnitInfo() = delete;
    UnitInfo(qint32, qint32, qint32);
};

class UnitStatus {
    qreal getHP() const { return uInfo.HPfull * uInfo.HPratio * HPnow; }
    qreal getCE() const { return uInfo.CEfull * uInfo.CEratio * HPnow; }
    qreal getMP() const { return uInfo.HPfull; }
    // return true if HP < 0
    bool  changeHP(qreal delta);

    const UnitInfo &uInfo;
    qreal           HPnow;  // [0,1]
    UnitStatus(const UnitInfo &);
};

class Unit : public QObject {
    Q_OBJECT
    UnitStatus unitStatus;

};

#endif