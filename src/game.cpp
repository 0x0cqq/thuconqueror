#include "game.h"
#include <QRandomGenerator>
#include <QThread>

Game::Game(GraphView *graphView, QPoint map_size, QObject *parent)
    : QObject(parent) {
    m_gameInfo.m_turnNumber = 0, m_gameInfo.map_size = map_size,
    m_gameInfo.nowPlayer = 1, m_gameInfo.playerNumbers = 2;
    nextTurnButtonWidget = nullptr;
    m_blocks.resize(width() + 2);
    for(int i = 1; i <= width(); i++) {
        m_blocks[i].resize(height() + 2);
        for(int j = 1; j <= height(); j++) {
            m_blocks[i][j] = new BlockStatus(
                QRandomGenerator::global()->generate() % 4 != 0 ? plainBlock :
                                                                  obstacleBlock,
                QPoint(i, j));
        }
    }
    m_field = new Field(m_gameInfo, m_blocks, m_units);
    m_graph = new GraphField(m_gameInfo, m_blocks, m_units);
    m_view  = graphView;
    if(m_view != nullptr) {
        m_view->setScene(m_graph);
    }

    connect(m_view, &GraphView::finishPainting, this, &Game::setButtonPos);
}

void Game::setButtonPos() {
    if(nextTurnButtonWidget != nullptr) {
        nextTurnButtonWidget->setPos(m_view->mapToScene(QPoint(100, 100)));
    }
}

void Game::init() {
    // connect(m_graph, &GraphField::userNewUnit,m_field, &Field::newUnit);
    connect(m_graph, &GraphField::userMoveUnit, m_field,
            QOverload<qint32, QPoint>::of(&Field::doUnitMove));
    connect(m_graph, &GraphField::userAttackUnit, m_field,
            QOverload<qint32, QPoint>::of(&Field::doUnitAttack));
    connect(m_graph, &GraphField::userShowMoveRange, m_field,
            &Field::getUnitMoveRange);

    connect(m_field, &Field::newUnit, m_graph, &GraphField::newUnit);
    connect(m_field, &Field::unitDead, m_graph, &GraphField::dieUnit);
    connect(
        m_field, &Field::moveUnit, m_graph,
        QOverload<qint32, const QVector<QPoint> &>::of(&GraphField::moveUnit));
    connect(m_field, &Field::attackUnit, m_graph,
            QOverload<qint32, qint32>::of(&GraphField::attackUnit));
    connect(m_field, &Field::unitMoveRangegot, m_graph,
            &GraphField::showMoveRange);

    connect(m_graph, &GraphField::userHideMoveRange, m_graph,
            &GraphField::hideMoveRange);
}

void Game::updateDetailedStatus(QLabel *detailedLabel) {
    if(m_graph->m_nowCheckedBlock == nullptr) {
        detailedLabel->setText("当前无选中格。");
    }
    else {
        QString text = "当前选中格为：(" +
            QString::number(m_graph->m_nowCheckedBlock->coord().x()) + "," +
            QString::number(m_graph->m_nowCheckedBlock->coord().y()) + ")\n";
        switch(m_graph->m_nowCheckedBlock->m_status->m_type) {
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

void Game::nextTurn() {
    if(m_gameInfo.playerNumbers == m_gameInfo.nowPlayer) {
        m_gameInfo.nowPlayer = 1;
        m_gameInfo.m_turnNumber++;
    }
    else {
        m_gameInfo.nowPlayer += 1;
    }
    QMessageBox msgBox;
    msgBox.setText("进入下一玩家游戏。当前是第 " +
                   QString::number(m_gameInfo.m_turnNumber) + " 回合，第 " +
                   QString::number(m_gameInfo.nowPlayer) +
                   " 号玩家，请开始操控。");
    msgBox.exec();
    emit gameStatusUpdated();
}