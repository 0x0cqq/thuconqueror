#include "enemyai.h"

#include "game.h"

#include <QEventLoop>
#include <QRandomGenerator>

EnemyAI::EnemyAI(Game *game, qint32 player) : m_game(game), m_player(player) {}

void EnemyAI::play() {
    Q_ASSERT(m_player == m_game->m_gameInfo.nowPlayer);
    // m_game->m_gameInfo.speed /= 10;
    bool       is_finished = false;
    QEventLoop waiter;
    auto       connection = connect(m_game->m_graph, &GraphField::finishOrder,
                              &waiter, &QEventLoop::quit);
    auto connection2 = connect(m_game->m_graph, &GraphField::finishOrder, this,
                               [&]() { is_finished = true; });
    while(moveUnit()) {
        if(!is_finished)
            waiter.exec();
        is_finished = false;
    }
    while(newUnit()) {
        ;
    }
    disconnect(connection);
    disconnect(connection2);

    // m_game->m_gameInfo.speed *= 10;
}

// 用 GraphField::onBlockClicked 来进行操作

bool EnemyAI::moveUnit() {
    // 先往敌人最多的地方进行移动
    for(int i = 0; i < m_game->m_units.size(); i++) {
        auto unitStatus = m_game->m_units[i];
        if(unitStatus->m_player != m_player)
            continue;
        if(!unitStatus->isAlive())
            continue;
        if(!unitStatus->canAttack() && !unitStatus->canMove())
            continue;
        QPoint p;
        // 能攻击就先攻击
        p = m_game->m_field->__canAttackNearby(unitStatus->m_uid);
        if(unitStatus->canAttack() && p != QPoint(-1, -1)) {
            doUnitAttack(unitStatus->m_uid, p);
            return true;
        }
        p = findTargetBlock(unitStatus->m_uid);
        if(unitStatus->canMove() && p != QPoint(-1, -1)) {
            qDebug() << "Move:" << unitStatus->m_uid << " " << p;
            doUnitMove(unitStatus->m_uid, p);
            // 理论上应该这里再试图攻击一次的。
            return true;
        }
    }
    return false;
}

// 计算一个格子附近的敌人数目
// 不太会算，见鬼
qreal EnemyAI::calculate(QPoint coord) {
    qreal ans = 0;
    for(int i = 0; i < m_game->m_units.size(); i++) {
        if(m_game->m_units[i]->m_player != m_player) {
            ans -= (abs(coord.x() - m_game->m_units[i]->m_nowCoord.x()) +
                    abs(coord.y() - m_game->m_units[i]->m_nowCoord.y()));
        }
    }
    for(int i = 1; i <= m_game->width(); i++) {
        for(int j = 1; j <= m_game->height(); j++) {
            if(m_game->m_blocks[i][j]->m_type == peopleCampBlock) {
                ans -= 50 * (abs(coord.x() - i) + abs(coord.y() - j));
            }
        }
    }
    return ans;
}

// 找到能去的格子里敌人最多的
QPoint EnemyAI::findTargetBlock(qint32 uid) {
    QVector<QPoint> moveRange = m_game->m_field->__getUnitMoveRange(uid);
    if(moveRange.size() == 0)
        return QPoint{-1, -1};
    QPoint ans = moveRange[0];
    for(int i = 1; i < moveRange.size(); i++) {
        if(calculate(moveRange[i]) > calculate(ans)) {
            ans = moveRange[i];
        }
    }
    return ans;
}

void EnemyAI::doUnitMove(qint32 uid, QPoint coord) {
    Q_ASSERT(m_game->m_units[uid]->isAlive());
    Q_ASSERT(m_game->m_units[uid]->m_player == m_player);
    Q_ASSERT(m_game->m_units[uid]->canMove());
    m_game->m_graph->onBlockClicked(m_game->m_units[uid]->m_nowCoord);
    Q_ASSERT(m_game->m_graph->blocks(coord)->m_isMoveRange);

    // _sleep(10000);
    m_game->m_graph->onBlockClicked(coord);
    // Q_ASSERT(m_game->m_units[uid]->m_nowCoord == coord);
}
void EnemyAI::doUnitAttack(qint32 uid, QPoint coord) {
    Q_ASSERT(m_game->m_units[uid]->isAlive());
    Q_ASSERT(m_game->m_units[uid]->m_player == m_player);
    Q_ASSERT(m_game->m_units[uid]->canAttack());
    // 如何确定一定能攻击呢？
    m_game->m_graph->onBlockClicked(m_game->m_units[uid]->m_nowCoord);
    m_game->m_graph->onBlockClicked(coord);
}

UnitType RandomType() {
    int t = (QRandomGenerator::global()->generate() % 5);
    switch(t) {
        case 0:
        case 1:
            return alphaUnit;
        case 2:
        case 3:
            return deltaUnit;
        case 4:
            return zetaUnit;
    }
}

bool EnemyAI::newUnit() {
    for(int i = 1; i <= m_game->width(); i++) {
        for(int j = 1; j <= m_game->height(); j++) {
            if(!((m_player == 1 &&
                  m_game->m_blocks[i][j]->m_type == peopleCampBlock) ||
                 (m_player == 2 &&
                  m_game->m_blocks[i][j]->m_type == virusCampBlock)))
                continue;
            auto blockStatus = m_game->m_blocks[i][j];
            if(blockStatus->m_unitOnBlock != -1)
                continue;
            // 告诉他们，爷要开始新建 Unit 了！
            doNewUnit(QPoint{i, j},
                      m_player == 1 ? studentUnit : RandomType());
            return true;
        }
    }
    return false;
}

void EnemyAI::doNewUnit(QPoint coord, UnitType type) {
    m_game->usernewUnit(coord, type);
}