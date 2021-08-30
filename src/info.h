#ifndef INFO_H
#define INFO_H

#include <QPolygon>
#include <QtGlobal>
#include <QtMath>

namespace GraphInfo {
const static qreal     blockSize = 100.0;
const static qreal     unitSize  = qSqrt(3) / 2 * blockSize;
const QPolygonF blockPoly =
    QVector<QPointF>{QPointF{-0.5, qSqrt(3) / 2} * blockSize,
                     QPointF{0.5, qSqrt(3) / 2} * blockSize,
                     QPointF{1.0, 0.0} * blockSize,
                     QPointF{0.5, -qSqrt(3) / 2} * blockSize,
                     QPointF{-0.5, -qSqrt(3) / 2} * blockSize,
                     QPointF{-1.0, 0.0} * blockSize};
const qreal penWidth = 5;

}  // namespace GraphInfo

#endif