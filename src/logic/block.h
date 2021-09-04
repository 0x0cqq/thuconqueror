#ifndef BLOCK_H
#define BLOCK_H

#include "../basic/info.h"
#include "../basic/status.h"
#include <QObject>

class Field;

class Block : public QObject {
    Q_OBJECT
  public:
    BlockStatus *m_status;

  public:
    Block(BlockStatus *status, QObject *parent = nullptr)
        : QObject(parent), m_status(status) {}
  public slots:
};

#endif