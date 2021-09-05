#ifndef GAME_H
#define GAME_H

#include "basic/status.h"
#include "graph/graphfield.h"
#include "graphview.h"
#include "logic/field.h"
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QString>

// 总体思路：所有实质性的内容都交给 Field 和 GraphField 去做，而且它们之间通过
// signals/slots 联系，不通过 Game 类联系。 所以，Game
// 类的任务就是推动游戏进一步进行。
class Game : public QObject {
    Q_OBJECT
  public:
    GameInfo            m_gameInfo;
    QMap<int, UnitInfo> m_typeInfo;
    // 两个玩家（or 一个玩家 vs 一个 AI）都玩完才算一个回合

    QVector<QVector<BlockStatus *>> m_blocks;
    QVector<UnitStatus *>           m_units;
    Field *                         m_field;
    GraphField *                    m_graph;
    GraphView *                     m_view;
    QGraphicsProxyWidget *          nextTurnButtonWidget;
    QGraphicsProxyWidget *          newUnitButtonWidget;
    QGraphicsProxyWidget *          pauseButtonWidget;
    QGraphicsProxyWidget *          policyTreeButtonWidget;
    QGraphicsProxyWidget *          gameStatusLabelWidget;

    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    Game(QPoint map_size, QObject *parent = nullptr);
    Game(const QJsonObject &json);
    Game(const QString &filename);
    ~Game();

    void clearMemory();
    void read(const QJsonObject &json);
    void write(QJsonObject &json);
  signals:
    void gameStatusUpdated();
  public slots:
    void init();

    void setgameStatusLabel();
    void updateGameStatus(QLabel *gameStatusLabel);
    void setDetailedLabel(QLabel *detailedLabel);
    void updateDetailedStatus(QLabel *detailedLabel);

    void setFixedWidgetPos();
    void setNextTurnButton();
    void usernextTurn();
    void setNewUnitButton();
    void usernewUnit();
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