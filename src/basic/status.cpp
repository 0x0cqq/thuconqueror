#include "status.h"
#include <QJsonArray>

void GameInfo::read(const QJsonObject &json) {
    if(json.contains("map_size") && json["map_size"].isArray())
        map_size = QPoint{json["map_size"].toArray()[0].toInt(),
                          json["map_size"].toArray()[1].toInt()};
    if(json.contains("nowPlayer") && json["nowPlayer"].isDouble())
        nowPlayer = UnitType(json["nowPlayer"].toInt());
    if(json.contains("turnNumber") && json["turnNumber"].isDouble())
        m_turnNumber = json["turnNumber"].toInt();
    if(json.contains("playerNumbers") && json["playerNumbers"].isDouble())
        playerNumbers = json["playerNumbers"].toInt();
}

void GameInfo::write(QJsonObject &json) {
    QJsonArray size;
    size.append(map_size.x());
    size.append(map_size.y());
    json["map_size"]      = size;
    json["nowPlayer"]     = nowPlayer;
    json["turnNumber"]    = m_turnNumber;
    json["playerNumbers"] = playerNumbers;
}

void BlockStatus::write(QJsonObject &json) {
    json["type"] = m_type;
    QJsonArray coord;
    coord.append(m_coord.x());
    coord.append(m_coord.y());
    json["coord"]       = coord;
    json["unitOnBlock"] = m_unitOnBlock;
}
void BlockStatus::read(const QJsonObject &json) {
    if(json.contains("type") && json["type"].isDouble())
        m_type = BlockType(json["type"].toInt());
    if(json.contains("coord") && json["coord"].isArray())
        m_coord = QPoint{json["coord"].toArray()[0].toInt(),
                         json["coord"].toArray()[1].toInt()};
    if(json.contains("unitOnBlock") && json["unitOnBlock"].isDouble())
        m_unitOnBlock = json["unitOnBlock"].toInt();
}

UnitStatus::UnitStatus(const int &uid, const UnitType type, UnitInfo *uInfo,
                       qint32 player, QPoint coord)
    : m_uid(uid), m_info(uInfo), m_type(type), m_player(player),
      m_nowCoord(coord), m_HPnow(1), m_canMove(true), m_canAttack(true) {}

UnitStatus::UnitStatus() {}

bool UnitStatus::changeHP(qreal delta) {
    Q_ASSERT(m_info->HPfull != 0);
    Q_ASSERT(m_info->HPratio != 0);
    m_HPnow += delta / (m_info->HPfull * m_info->HPratio);
    return m_HPnow < 0;
}

void UnitStatus::read(const QJsonObject &json) {
    if(json.contains("uid") && json["uid"].isDouble())
        m_uid = json["uid"].toInt();
    // if(json.contains("info") && json["info"].isObject())
    //     m_info.read(json["info"].toObject());
    // ???
    if(json.contains("type") && json["type"].isDouble())
        m_type = UnitType(json["type"].toInt());
    if(json.contains("player") && json["player"].isDouble())
        m_player = json["player"].toInt();
    if(json.contains("nowCoord") && json["nowCoord"].isArray())
        m_nowCoord = QPoint{json["nowCoord"].toArray()[0].toInt(),
                            json["nowCoord"].toArray()[1].toInt()};
    if(json.contains("HPnow") && json["HPnow"].isDouble())
        m_HPnow = json["HPnow"].toDouble();
    if(json.contains("canMove") && json["canMove"].isBool())
        m_canMove = json["canMove"].toBool();
    if(json.contains("canAttack") && json["canAttack"].isBool())
        m_canAttack = json["canAttack"].toBool();
}

void UnitStatus::write(QJsonObject &json) const {
    json["uid"] = m_uid;
    // QJsonObject info;
    // m_info.write(info);
    // json["info"]   = info;
    json["type"]   = m_type;
    json["player"] = m_player;
    QJsonArray nowCoord;
    nowCoord.append(m_nowCoord.x());
    nowCoord.append(m_nowCoord.y());
    json["nowCoord"]  = nowCoord;
    json["HPnow"]     = m_HPnow;
    json["canMove"]   = m_canMove;
    json["canAttack"] = m_canAttack;
}

void UnitStatus::setMoveState(bool state) {
    m_canMove = state;
    emit unitStateChanged();
}
void UnitStatus::setAttackState(bool state) {
    m_canAttack = state;
    emit unitStateChanged();
}

QPair<qreal, qreal> calculateAttack(UnitStatus *source, UnitStatus *target) {
    qreal a = source->getCE(), b = target->getCE();
    return qMakePair(b, a);
}

QPoint nearby[2][6] = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 1}},
                       {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}}};

bool isNearByPoint(const QPoint &a, const QPoint &b) {
    for(int i = 0; i < 6; i++) {
        if(a + nearby[a.y() % 2][i] == b) {
            return true;
        }
    }
    return false;
}

bool isNearByBlock(const BlockStatus *a, const BlockStatus *b) {
    return isNearByPoint(a->m_coord, b->m_coord);
}

bool canUnitAttack(const UnitStatus *a, const UnitStatus *b) {
    // 没有检查 a 的主人是什么
    if(!isNearByPoint(a->m_nowCoord, b->m_nowCoord)) {
        return false;
    }
    if(!a->isAlive() || !b->isAlive()) {
        return false;
    }
    if(a->m_player == b->m_player) {
        return false;
    }
    if(!a->canAttack()) {
        return false;
    }
    return true;
}

QVector<QPoint> getNearbyPoint(const QPoint &a) {
    QVector<QPoint> ans;
    for(int i = 0; i < 6; i++) {
        ans.push_back(a + nearby[a.y() % 2][i]);
    }
    return ans;
}

QPointF getBlockCenter(qint32 r, qint32 c) {
    // Q_ASSERT(1 <= r && r <= width());
    // Q_ASSERT(1 <= c && c <= height());
    return QPointF(qSqrt(3) * (r - 1) + (c % 2 == 0 ? qSqrt(3) / 2 : 0),
                   1.5 * (c - 1)) *
        GraphInfo::blockSize;
}

QPointF getBlockCenter(QPoint coord) {
    return getBlockCenter(coord.x(), coord.y());
}
