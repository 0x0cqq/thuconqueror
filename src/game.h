#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "graphfield.h"

#include <QObject>

class GameInfo{

};

class Game : public QObject {
    Q_OBJECT
    Field *m_field;
    GraphField *m_graph;

};

#endif