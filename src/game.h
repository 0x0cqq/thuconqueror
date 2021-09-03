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
    void setgameStatusLabel(QLabel *gameStatusLabel) {
        // 应该重载一下那个 Label的，不过之后再说吧，现在先写一个能用的
        connect(this, &Game::gameStatusUpdated, this,
                [=]() { this->updateGameStatus(gameStatusLabel); });
        this->updateGameStatus(gameStatusLabel);
    }
    void updateGameStatus(QLabel *gameStatusLabel) {
        gameStatusLabel->setText(
            "回合数：" + QString::number(m_gameInfo.m_turnNumber) +
            " 现在正在操作的玩家：" + QString::number(m_gameInfo.nowPlayer));
    }
    void setDetailedLabel(QLabel *detailedLabel) {
        connect(m_graph, &GraphField::checkStateChange, this,
                [=]() { this->updateDetailedStatus(detailedLabel); });
        connect(m_graph, &GraphField::needUpdateDetail, this,
                [=]() { this->updateDetailedStatus(detailedLabel); });
        this->updateDetailedStatus(detailedLabel);
    }
    void updateDetailedStatus(QLabel *detailedLabel);

    void setNextTurnButton() {
        QPushButton *nextTurnButton = new QPushButton();
        nextTurnButton->setIcon(QIcon(":/icons/nextturn.svg"));
        nextTurnButton->setWhatsThis("当前回合完成，进入下一回合");

        nextTurnButton->setIconSize(QSize(85, 85));
        nextTurnButton->setContentsMargins(5, 5, 10, 10);
        nextTurnButtonWidget = m_graph->addWidget(nextTurnButton);
        nextTurnButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                      true);

        nextTurnButtonWidget->setGeometry(
            QRect(QPoint(0, 0), QPoint(100, 100)));
        connect(nextTurnButton, &QPushButton::clicked, this, &Game::nextTurn);
        emit gameStatusUpdated();
    }
    void setNewUnitButton() {
        QPushButton *newUnitButton = new QPushButton();
        newUnitButton->setText("新建兵");
        newUnitButton->setWhatsThis("在选中的Block上新建一个兵吧！");
        newUnitButtonWidget = m_graph->addWidget(newUnitButton);
        newUnitButtonWidget->setGeometry(
            QRect(QPoint(0, 0), QPoint(100, 100)));
        newUnitButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                     true);
        connect(newUnitButton, &QPushButton::clicked, this, [=]() {
            // 需要当前位置没有Unit，否则会炸掉的
            if(m_graph->m_nowCheckedBlock == nullptr) {
                QMessageBox msgBox;
                msgBox.setText("没有选中Block!");
                msgBox.exec();
                return;
            }
            if(m_graph->m_nowCheckedBlock->unitOnBlock() != -1) {
                QMessageBox msgBox;
                msgBox.setText("当前Block已经有Unit了!");
                msgBox.exec();
                return;
            }
            if(m_graph->m_nowCheckedBlock->m_status->m_type != plainBlock) {
                QMessageBox msgBox;
                msgBox.setText("当前 Block 的地形不能生产 Unit !");
                msgBox.exec();
                return;
            }
            UnitStatus *unitStatus = new UnitStatus(
                m_units.size(), virusUnit, unitinfo, m_gameInfo.nowPlayer,
                m_graph->m_nowCheckedBlock->coord());

            emit m_graph->checkStateChange(m_graph->m_nowCheckedBlock->coord(),
                                           false);
            m_graph->m_nowCheckedBlock = nullptr;
            m_units.push_back(unitStatus);
            m_field->doNewUnit(unitStatus);
        });
    }
    void nextTurn();
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