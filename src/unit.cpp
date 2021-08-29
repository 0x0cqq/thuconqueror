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
    HPnow -= delta / (uInfo.HPfull * uInfo.HPratio);
    return HPnow < 0;
}