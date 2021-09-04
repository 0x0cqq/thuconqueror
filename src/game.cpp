#include "game.h"
#include <QRandomGenerator>
#include <QThread>
#include <QWindow>

Game::Game(GraphView *graphView, QPoint map_size, QObject *parent)
    : QObject(parent) {
    m_gameInfo.m_turnNumber = 0, m_gameInfo.map_size = map_size,
    m_gameInfo.nowPlayer = 1, m_gameInfo.playerNumbers = 2;
    nextTurnButtonWidget = nullptr, newUnitButtonWidget = nullptr,
    pauseButtonWidget = nullptr;
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
    // connect(m_view, &GraphView::finishPainting, this, &Game::setButtonPos);

    connect(m_graph, &GraphField::checkStateChange, this,
            [=](QPoint, bool state) {
                if(state == true) {
                    showNewUnitButton();
                }
                else {
                    hideNewUnitButton();
                }
            });
}

void Game::setgameStatusLabel(QLabel *gameStatusLabel) {
    // 应该重载一下那个 Label的，不过之后再说吧，现在先写一个能用的
    connect(this, &Game::gameStatusUpdated, this,
            [=]() { this->updateGameStatus(gameStatusLabel); });
    this->updateGameStatus(gameStatusLabel);
}

void Game::updateGameStatus(QLabel *gameStatusLabel) {
    gameStatusLabel->setText(
        "回合数：" + QString::number(m_gameInfo.m_turnNumber) +
        " 现在正在操作的玩家：" + QString::number(m_gameInfo.nowPlayer));
}

void Game::setButtonPos() {
    if(nextTurnButtonWidget != nullptr) {
        nextTurnButtonWidget->setPos(m_view->mapToScene(
            QPoint(m_view->size().width(), m_view->height()) -
            QPoint(100, 100)));
        nextTurnButtonWidget->setZValue(100);
    }
    if(newUnitButtonWidget != nullptr) {
        newUnitButtonWidget->setPos(m_view->mapToScene(
            QPoint(m_view->size().width() / 2 - 50, m_view->height() - 100)));
        newUnitButtonWidget->setZValue(100);
    }
    if(pauseButtonWidget != nullptr) {
        pauseButtonWidget->setPos(
            m_view->mapToScene(QPoint(m_view->size().width() - 100, 0)));
        pauseButtonWidget->setZValue(100);
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

void Game::setDetailedLabel(QLabel *detailedLabel) {
    connect(m_graph, &GraphField::checkStateChange, this,
            [=]() { this->updateDetailedStatus(detailedLabel); });
    connect(m_graph, &GraphField::needUpdateDetail, this,
            [=]() { this->updateDetailedStatus(detailedLabel); });
    this->updateDetailedStatus(detailedLabel);
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

void Game::setNextTurnButton() {
    QPushButton *nextTurnButton = new QPushButton();
    nextTurnButton->setIcon(QIcon(":/icons/nextturn.svg"));
    nextTurnButton->setWhatsThis("当前回合完成，进入下一回合");

    nextTurnButton->setIconSize(QSize(85, 85));
    nextTurnButton->setContentsMargins(5, 5, 10, 10);
    nextTurnButtonWidget = m_graph->addWidget(nextTurnButton);
    nextTurnButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                  true);

    nextTurnButtonWidget->setGeometry(QRect(QPoint(0, 0), QPoint(100, 100)));
    connect(nextTurnButton, &QPushButton::clicked, this, &Game::usernextTurn);
    emit gameStatusUpdated();
}

void Game::usernextTurn() {
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

void Game::setPauseButton() {
    QPushButton *pauseButton = new QPushButton();
    pauseButton->setGeometry(-100, 0, 100, 100);
    pauseButton->setIcon(QIcon(":/icons/pause.png"));
    pauseButton->setIconSize(QSize(85, 85));
    pauseButton->setContentsMargins(5, 5, 10, 10);
    pauseButtonWidget = m_graph->addWidget(pauseButton);
    pauseButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    connect(pauseButton, &QPushButton::clicked, this, &Game::userPause);
}

void Game::userPause() {
    QDialog window;
    window.exec();
    // window->show();
    // m_graph->addWidget(window);
}

void Game::setNewUnitButton() {
    QPushButton *newUnitButton = new QPushButton();
    newUnitButton->setText("新建兵");
    newUnitButton->setWhatsThis("在选中的Block上新建一个兵吧！");
    newUnitButtonWidget = m_graph->addWidget(newUnitButton);
    newUnitButtonWidget->setGeometry(QRect(QPoint(0, 0), QPoint(100, 100)));
    newUnitButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                 true);
    connect(newUnitButton, &QPushButton::clicked, this, &Game::usernewUnit);
}

void Game::usernewUnit() {
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

    emit m_graph->checkStateChange(m_graph->m_nowCheckedBlock->coord(), false);
    m_graph->m_nowCheckedBlock = nullptr;
    m_units.push_back(unitStatus);
    m_field->doNewUnit(unitStatus);
}