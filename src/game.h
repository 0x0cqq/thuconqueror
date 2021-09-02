#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "graphfield.h"
#include "graphview.h"
#include "info.h"
#include "status.h"
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
    qint32 width() const { return m_gameInfo.map_size.x(); }
    qint32 height() const { return m_gameInfo.map_size.y(); }
    Game(GraphView *graphView, QPoint map_size, QObject *parent = nullptr);

    // game 类得有一个专门的函数负责把两边连接起来，其实主要是 死的 graphblock
    // 和 abstractblock 之间。这个函数不太好写，之后再说吧。
  signals:
    void gameStatusUpdated();
  public slots:
    void init() {
        // connect(m_graph, &GraphField::userNewUnit,m_field, &Field::newUnit);
        connect(m_graph, &GraphField::userMoveUnit, m_field,
                QOverload<qint32, QPoint>::of(&Field::doUnitMove));
        connect(m_graph, &GraphField::userAttackUnit, m_field,
                QOverload<qint32, QPoint>::of(&Field::doUnitAttack));
        connect(m_graph, &GraphField::userShowMoveRange, m_field,
                &Field::getUnitMoveRange);

        connect(m_field, &Field::newUnit, m_graph, &GraphField::newUnit);
        connect(m_field, &Field::unitDead, m_graph, &GraphField::dieUnit);
        connect(m_field, &Field::moveUnit, m_graph,
                QOverload<qint32, const QVector<QPoint> &>::of(
                    &GraphField::moveUnit));
        connect(m_field, &Field::attackUnit, m_graph,
                QOverload<qint32, qint32>::of(&GraphField::attackUnit));
        connect(m_field, &Field::unitMoveRangegot, m_graph,
                &GraphField::showMoveRange);

        connect(m_graph, &GraphField::userHideMoveRange, m_graph,
                &GraphField::hideMoveRange);
    }
    // 一定要采用这种方式吗？或许之后可以把所有的 GUI 都塞到图里面呢。
    void setNewUnitButton(QPushButton *newUnitButton) {
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
            if(m_graph->m_nowCheckedBlock->m_status->m_type != plainBlock){
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
    void setgameStatusLabel(QLabel *gameStatusLabel) {
        // 应该重载一下那个 Label的，不过之后再说吧，现在先写一个能用的
        connect(this, &Game::gameStatusUpdated, this,
                [=]() { this->updateGameStatus(gameStatusLabel); });
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
    void updateDetailedStatus(QLabel *detailedLabel) {
        if(m_graph->m_nowCheckedBlock == nullptr) {
            detailedLabel->setText("当前无选中格。");
        }
        else {
            QString text = "当前选中格为：(" +
                QString::number(m_graph->m_nowCheckedBlock->coord().x()) + "," +
                QString::number(m_graph->m_nowCheckedBlock->coord().y()) +
                ")\n";
            switch(m_graph->m_nowCheckedBlock->m_status->m_type){
                case plainBlock:
                    text.append("当前地形为平原，可以生产 Unit。\n");
                    break;
                case obstacleBlock:
                    text.append("当前地形为障碍，不能生产、通过 Unit。\n");
                    break;
                default:
                    text.append("是的，这是一个bug。\n");
                    break;
            }
            if(m_graph->m_nowCheckedBlock->unitOnBlock() == -1) {
                text.append("当前格上无单元。");
            }
            else {
                text.append(
                    "当前格上单元编号：" +
                    QString::number(m_graph->m_nowCheckedBlock->unitOnBlock()) +
                    "。\n");
                UnitStatus *unitStatus =
                    m_units[m_graph->m_nowCheckedBlock->unitOnBlock()];
                text.append("血量：" + QString::number(unitStatus->getHP()) +
                            " 移动力：" + QString::number(unitStatus->getMP()) +
                            " 攻击力：" + QString::number(unitStatus->getCE()));
            }
            detailedLabel->setText(text);
        }
    }

    void setNextTurnButton(QPushButton *nextTurnButton) {
        connect(nextTurnButton, &QPushButton::clicked, this, &Game::nextTurn);
        emit gameStatusUpdated();
    }
    void nextTurn();
};

#endif