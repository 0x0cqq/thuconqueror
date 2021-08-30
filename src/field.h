#ifndef FIELD_H
#define FIELD_H


#include "block.h"
#include <QObject>
#include <QList>

class FieldInfo {
    int width, height;
};



class Field : public QObject {
    Q_OBJECT
    FieldInfo fieldInfo;
    QList<QList<Block>> blocks;
    QList<Unit> units; 
};

#endif