#ifndef FIELD_H
#define FIELD_H

#include "block.h"
#include <QList>
#include <QObject>

class FieldInfo {
    int _width, _height;

  public:
    FieldInfo(const QPoint &fieldSize)
        : _width(fieldSize.x()), _height(fieldSize.y()) {}
    qint32 width() const { return _width; }
    qint32 height() const { return _height; }
};

class Field : public QObject {
    Q_OBJECT
    FieldInfo                 fieldInfo;
    QVector<QVector<Block *>> blocks;
    QList<Unit *>             units;

    qint32 width() const { return fieldInfo.width(); }
    qint32 height() const { return fieldInfo.height(); }

    Field(const QPoint &fieldSize);
};

#endif