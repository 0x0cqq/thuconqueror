#ifndef GAME_H
#define GAME_H

#include "../basic/status.h"
#include "../graph/graphfield.h"
#include "../graph/menudialog.h"
#include "../logic/field.h"
#include "graphview.h"
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QString>

class EnemyAI;

// 总体思路：所有实质性的内容都交给 Field 和 GraphField 去做，而且它们之间通过
// signals/slots 联系，不通过 Game 类联系。 所以，Game
// 类的任务就是推动游戏进一步进行。
class Game : public GraphView {
    Q_OBJECT
  public:
    bool                             readyToPlay;
    QVector<QMetaObject::Connection> connection;
    GameInfo                         m_gameInfo;
    QMap<int, UnitInfo>              m_unitTypeInfo;
    QMap<int, BlockInfo>             m_blockTypeInfo;

    EnemyAI *enemy;
    // 两个玩家（or 一个玩家 vs 一个 AI）都玩完才算一个回合

    QVector<QVector<BlockStatus *>> m_blocks;
    QVector<UnitStatus *>           m_units;
    Field *                         m_field;
    GraphField *                    m_graph;
    // QGraphicsProxyWidget *          newUnitDialogWidget;
    QGraphicsProxyWidget *nextTurnButtonWidget;
    QGraphicsProxyWidget *newUnitButtonWidget;
    QGraphicsProxyWidget *pauseButtonWidget;
    QGraphicsProxyWidget *policyTreeButtonWidget;
    QGraphicsProxyWidget *gameStatusLabelWidget;

    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    // Game(QPoint map_size, QObject *parent);
    Game(const QJsonObject &json, QWidget *parent);
    Game(const QString &filename, QWidget *parent);
    Game(const qint32 &level, QWidget *parent);
    ~Game();

    // 处理内存和 connection
    void clearMemory();
    void clearConnection();
    void read(const QJsonObject &json);
    void write(QJsonObject &json);
    void setView();

  signals:
    //游戏开始和结束
    void gameStatusUpdated();
    void lose(int player);
    void loadGame(QString fileName);
  public slots:
    // 一些初始化
    void init();
    void setgameStatusLabel();
    void updateGameStatus(QLabel *gameStatusLabel);
    void setDetailedLabel(QLabel *detailedLabel);
    void updateDetailedStatus(QLabel *detailedLabel);

    void setFixedWidgetPos();
    void setNextTurnButton();
    void usernextTurn();
    void setNewUnitButton();
    void usernewUnit(QPoint coord, UnitType newUnitType);
    void setPauseButton();
    void userPause();
    void setPolicyTreeButton();
    void usershowPolicyTree();
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