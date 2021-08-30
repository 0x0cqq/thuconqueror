#include "unit.h"

UnitInfo::UnitInfo() {
    HPfull = CEfull = MPfull = 0;
    HPratio = CEratio = 1;
}

UnitInfo::UnitInfo(qint32 _HPfull, qint32 _CEfull, qint32 _MPfull)
    : HPfull(_HPfull), CEfull(_CEfull), MPfull(_MPfull) {
    HPratio = CEratio = 1;
}

UnitStatus::UnitStatus(const UnitInfo &_uInfo) : uInfo(_uInfo), HPnow(1.0) {}

bool UnitStatus::changeHP(qreal delta) {
    Q_ASSERT(uInfo.HPfull != 0);
    Q_ASSERT(uInfo.HPratio != 0);
    HPnow += delta / (uInfo.HPfull * uInfo.HPratio);
    return HPnow < 0;
}

// calculate the attack point (source, target)
QPair<qreal,qreal> Unit::calculateAttack(Unit *target) {
    qreal a = this->unitStatus.getCE(), b = target->unitStatus.getCE();
    return qMakePair(b,a);
}

bool Unit::isAlive() const {
    return this->unitStatus.getHP() < 0;
}

void Unit::attack(Unit *enemyUnit) {
    auto attHP = calculateAttack(enemyUnit);
    bool isADead = this->unitStatus.changeHP(-attHP.first);
    bool isBDead = this->unitStatus.changeHP(-attHP.first);
    if(isADead) {
        emit this->unitDead();
    }
    if(isBDead) {
        emit enemyUnit->unitDead();
    }
}