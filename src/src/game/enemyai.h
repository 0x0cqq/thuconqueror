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
    EnemyAI(Game *game, qint32 player, QObject *parent);
    void   play();
    // AI 移动单元
    bool   moveUnit();
    // AI 新建单元
    bool   newUnit();
    // 找到一个目标单元
    QPoint findTargetBlock(qint32 uid);
    // 根据敌军的情况来决定
    qreal  calculate(QPoint coord);
    // 模拟点击和攻击、新建
    void   doUnitMove(qint32 uid, QPoint coord);
    void   doUnitAttack(qint32 uid, QPoint coord);
    void   doNewUnit(QPoint coord, UnitType type);
};

#endif