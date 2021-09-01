#ifndef BLOCK_H
#define BLOCK_H

#include "info.h"
#include "status.h"
#include <QObject>
#include <QPoint>

class Field;

class Block : public QObject {
    Q_OBJECT
  public:
    BlockStatus *m_status;

  public:
    Block(BlockStatus *status, QObject *parent = nullptr)
        : QObject(parent), m_status(status) {}
    bool canContainUnit() const { return m_status->m_type | yesUnitBlock; }
  public slots:
};

#endif