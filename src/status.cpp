#include "status.h"

UnitStatus::UnitStatus(const int &uid, const UnitType type,
                       const UnitInfo &uInfo, qint32 player, QPoint coord)
    : m_info(uInfo), m_type(type), m_uid(uid), m_player(player),
      m_nowCoord(coord), m_HPnow(1) {}

bool UnitStatus::changeHP(qreal delta) {
    Q_ASSERT(m_info.HPfull != 0);
    Q_ASSERT(m_info.HPratio != 0);
    m_HPnow += delta / (m_info.HPfull * m_info.HPratio);
    return m_HPnow < 0;
}

QPair<qreal, qreal> calculateAttack(UnitStatus *source, UnitStatus *target) {
    qreal a = source->getCE(), b = target->getCE();
    return qMakePair(b, a);
}