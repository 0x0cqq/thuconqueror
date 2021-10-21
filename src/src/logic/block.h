#ifndef BLOCK_H
#define BLOCK_H

#include "../basic/status.h"
#include <QObject>

class Field;

class Block : public QObject {
    Q_OBJECT
  public:
    BlockStatus *m_status;

  public:
    Block(BlockStatus *status, QObject *parent)
        : QObject(parent), m_status(status) {}
  public slots:
};

#endif