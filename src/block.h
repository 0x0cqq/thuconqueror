#ifndef BLOCK_H
#define BLOCK_H

#include "unit.h"
#include <QObject>
#include <QPoint>

enum BlockType {
    yesUnitBlock    = 1,  // 可以去，而不是现在有没有
    noUnitBlock     = 1 << 1,
    plainBlock      = yesUnitBlock | 1 << 2,
    obstacleBlock   = noUnitBlock | 1 << 3,
    dampBlock       = yesUnitBlock | 1 << 4,
    virusCampBlock  = yesUnitBlock | 1 << 5,
    peopleCampBlock = yesUnitBlock | 1 << 6
};

class BlockStatus {
  public:
    BlockType blockType;
    qint32    MPneed() const {}
};

class Field;

class Block : public QObject {
    Q_OBJECT
    BlockStatus blockStatus;
    Unit *      unit;
    QPoint      pos;
    Field *     field;
    bool canContainUnit() const { return blockStatus.blockType | yesUnitBlock; }
};

#endif