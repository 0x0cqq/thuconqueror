#ifndef STATUS_H
#define STATUS_H

#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QPolygon>
#include <QtGlobal>
#include <QtMath>

namespace GraphInfo {
const static qreal blockSize = 100.0;
const static qreal unitSize  = qSqrt(3) / 2 * qSqrt(3) / 2 * blockSize;
const QPolygonF    blockPoly =
    QVector<QPointF>{QPointF{-0.5, qSqrt(3) / 2} * blockSize,
                     QPointF{0.5, qSqrt(3) / 2} * blockSize,
                     QPointF{1.0, 0.0} * blockSize,
                     QPointF{0.5, -qSqrt(3) / 2} * blockSize,
                     QPointF{-0.5, -qSqrt(3) / 2} * blockSize,
                     QPointF{-1.0, 0.0} * blockSize};
const qreal penWidth = 5;
}  // namespace GraphInfo

class GameInfo {
  public:
    QPoint map_size;
    qint32 m_turnNumber;
    // 人类 1 病毒 2
    qint32 playerNumbers;
    qint32 nowPlayer;
    void   read(const QJsonObject &json);
    void   write(QJsonObject &json);
};

enum BlockType {
    yesUnitBlock    = 1,  // 可以去（对双方），而不是现在有没有
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
    BlockType m_type;
    QPoint    m_coord;
    qint32    m_unitOnBlock;
    void      read(const QJsonObject &json);
    void      write(QJsonObject &json);
    qint32    MPneed() const { return 1; }
    BlockStatus() {}
    BlockStatus(BlockType type, QPoint coord)
        : m_type(type), m_coord(coord), m_unitOnBlock(-1) {}
};

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
    void   read(const QJsonObject &json) {
        if(json.contains("HPfull") && json["HPfull"].isDouble())
            HPfull = json["HPfull"].toInt();
        if(json.contains("HPratio") && json["HPratio"].isDouble())
            HPratio = json["HPratio"].toDouble();
        if(json.contains("CEfull") && json["CEfull"].isDouble())
            CEfull = json["CEfull"].toInt();
        if(json.contains("CEratio") && json["CEratio"].isDouble())
            CEratio = json["CEratio"].toDouble();
        if(json.contains("MPfull") && json["MPfull"].isDouble())
            MPfull = json["MPfull"].toInt();
    }
    void write(QJsonObject &json) {
        json["HPfull"]  = HPfull;
        json["HPratio"] = HPratio;
        json["CEfull"]  = CEfull;
        json["CEratio"] = CEratio;
        json["MPfull"]  = MPfull;
    }
    UnitInfo() {
        HPfull = CEfull = MPfull = 0;
        HPratio = CEratio = 1;
    }
    UnitInfo(qint32 _HPfull, qint32 _CEfull, qint32 _MPfull)
        : HPfull(_HPfull), CEfull(_CEfull), MPfull(_MPfull) {
        HPratio = CEratio = 1;
    }
};

enum UnitType {
    peopleUnit  = 1 << 1,
    virusUnit   = 1 << 2,
    studentUnit = peopleUnit | 1 << 3,
    teacherUnit = peopleUnit | 1 << 4,
    childUnit   = peopleUnit | 1 << 5,
    alphaUnit   = virusUnit | 1 << 6,
    deltaUnit   = virusUnit | 1 << 7,
    zetaUnit    = virusUnit | 1 << 8
    // setting: 第一位和第二位是判断类型的?
};

class UnitStatus : public QObject {
    Q_OBJECT
  public:
    // return true if HP < 0
    qint32    m_uid;
    UnitInfo &m_info;
    UnitType  m_type;
    qint32    m_player;
    QPoint    m_nowCoord;
    qreal     m_HPnow;  // [0,1]
    qreal     getHP() const { return m_info.HPfull * m_info.HPratio * m_HPnow; }
    qreal     getCE() const { return m_info.CEfull * m_info.CEratio * m_HPnow; }
    qreal     getMP() const { return m_info.MPfull; }
    bool      changeHP(qreal delta);
    bool      isAlive() const { return this->getHP() > 0; }
    void      read(const QJsonObject &json);
    void      write(QJsonObject &json);
    UnitStatus();
    UnitStatus(const int &uid, const UnitType type, UnitInfo &uInfo,
               qint32 player, QPoint coord);
};

// calculate the attack point (source, target)
QPair<qreal, qreal> calculateAttack(UnitStatus *source, UnitStatus *target);

bool isNearByPoint(const QPoint &a, const QPoint &b);

bool isNearbyBlock(const BlockStatus *a, const BlockStatus *b);

QVector<QPoint> getNearbyPoint(const QPoint &a);

#endif
