#ifndef UNIT_H
#define UNIT_H

#include "info.h"
#include "status.h"
#include <QObject>

class Unit : public QObject {
    Q_OBJECT
  public:
    UnitStatus *m_status;

    // qint32 uid() const { return m_status->m_uid; }
    // QPoint nowCoord() const { return m_status->m_nowCoord; }
    bool isVirusUnit() const { return m_status->m_type & virusUnit; }
    bool isPeopleUnit() const { return m_status->m_type & peopleUnit; }
    bool isSamePlayer(Unit *target) const {
        return m_status->m_type & target->m_status->m_type;
    }
    Unit(UnitStatus *unitStatus, QObject *parent = nullptr)
        : QObject(parent), m_status(unitStatus) {}
};

#endif