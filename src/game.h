#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "graphfield.h"
#include "graphview.h"
#include "info.h"
#include "status.h"
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QObject>
#include <QPoint>
#include <QPushButton>
#include <QString>

// 总体思路：所有实质性的内容都交给 Field 和 GraphField 去做，而且它们之间通过
// signals/slots 联系，不通过 Game 类联系。 所以，Game
// 类的任务就是推动游戏进一步进行。
class Game : public QObject {
    Q_OBJECT
  public:
    UnitInfo unitinfo = UnitInfo(10, 1, 5);
    GameInfo m_gameInfo;
    // 两个玩家（or 一个玩家 vs 一个 AI）都玩完才算一个回合

    QVector<QVector<BlockStatus *>> m_blocks;
    QVector<UnitStatus *>           m_units;
    Field *                         m_field;
    GraphField *                    m_graph;
    GraphView *                     m_view;
    QGraphicsProxyWidget *          nextTurnButtonWidget;
    QGraphicsProxyWidget *          newUnitButtonWidget;

    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    Game(GraphView *graphView, QPoint map_size, QObject *parent = nullptr);

  signals:
    void gameStatusUpdated();
  public slots:
    void init();
    void setButtonPos();
    void setgameStatusLabel(QLabel *gameStatusLabel);
    void updateGameStatus(QLabel *gameStatusLabel);
    void setDetailedLabel(QLabel *detailedLabel);
    void updateDetailedStatus(QLabel *detailedLabel);

    void setNextTurnButton();
    void setNewUnitButton();
    void usernextTurn();
    void usernewUnit();
    void showNewUnitButton() {
        if(newUnitButtonWidget != nullptr) {
            newUnitButtonWidget->show();
        }
    }
    void hideNewUnitButton() {
        if(newUnitButtonWidget != nullptr) {
            newUnitButtonWidget->hide();
        }
    }
};

#endif