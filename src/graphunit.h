#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H

#include <QGraphicsObject>

class Unit : public QGraphicsObject {
    Q_OBJECT
};

class PersonUnit : public Unit {
    Q_OBJECT
};

class VirusUnit : public Unit {
    Q_OBJECT
};

#endif  // UNIT_H