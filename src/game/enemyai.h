#ifndef ENEMYAI_H
#define ENEMYAI_H

#include "../basic/status.h"
#include <QObject>

class Game;

class EnemyAI : public QObject {
    Q_OBJECT
    Game *const m_game;

  public:
    const qint32 m_player;
    EnemyAI(Game *game, qint32 player);
    void   play();
    bool   moveUnit();
    QPoint findTargetBlock(qint32 uid);
    qreal  calculate(QPoint coord);
    void   doUnitMove(qint32 uid, QPoint coord);
    void   doUnitAttack(qint32 uid, QPoint coord);
    bool   newUnit();
    void   doNewUnit(QPoint coord, UnitType type);
};

#endif