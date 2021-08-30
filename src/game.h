#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "graphfield.h"
#include <QObject>

class GameInfo {
    qint32 m_width, m_height;
};

// 总体思路：所有实质性的内容都交给 Field 和 GraphField 去做，而且它们之间通过
// signals/slots 联系，不通过 Game 类联系。 所以，Game
// 类的任务就是推动游戏进一步进行。
class Game : public QObject {
    Q_OBJECT
    // 两个玩家（or 一个玩家 vs 一个 AI）都玩完才算一个回合
    qint32 m_turnNumber;
    // 人类 1 病毒 2
    qint32      nowPlayer;
    Field *     m_field;
    GraphField *m_graph;
    Game(QObject *parent = nullptr) : QObject(parent), m_turnNumber(0) {}
  signals:
    
  public slots:
};

#endif