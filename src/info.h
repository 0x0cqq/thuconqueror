#ifndef INFO_H
#define INFO_H

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
};

class FieldInfo {
    int _width, _height;

  public:
    FieldInfo(const QPoint &fieldSize)
        : _width(fieldSize.x()), _height(fieldSize.y()) {}
    qint32 width() const { return _width; }
    qint32 height() const { return _height; }
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
    childUnit   = peopleUnit | 1 << 5
    // setting: 第一位和第二位是判断类型的?
};

#endif