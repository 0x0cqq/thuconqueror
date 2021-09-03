#include "status.h"

UnitStatus::UnitStatus(const int &uid, const UnitType type,
                       const UnitInfo &uInfo, qint32 player, QPoint coord)
    : m_uid(uid), m_info(uInfo), m_type(type), m_player(player),
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

QPoint nearby[2][6] = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, 1}, {1, 1}},
                       {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, -1}}};

bool isNearByPoint(const QPoint &a, const QPoint &b) {
    for(int i = 0; i < 6; i++) {
        if(a + nearby[a.x() % 2][i] == b) {
            return true;
        }
    }
    return false;
}

bool isNearbyBlock(const BlockStatus *a, const BlockStatus *b) {
    return isNearByPoint(a->m_coord, b->m_coord);
}

QVector<QPoint> getNearbyPoint(const QPoint &a) {
    QVector<QPoint> ans;
    for(int i = 0; i < 6; i++) {
        ans.push_back(a + nearby[a.x() % 2][i]);
    }
    return ans;
}
