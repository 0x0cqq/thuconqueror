#include "game.h"
#include "graph/menudialog.h"
#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QString>
#include <QThread>
#include <QWindow>

// https://www.cnblogs.com/lifexy/p/11930436.html
bool openJson(const QString &filename, QJsonObject &json) {
    //打开文件
    QFile file(QApplication::applicationDirPath() + "/" + filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File open failed!";
    }
    else {
        qDebug() << "File open successfully!";
    }
    QJsonParseError *error = new QJsonParseError;
    QJsonDocument    jdc   = QJsonDocument::fromJson(file.readAll(), error);

    //判断文件是否完整
    if(error->error != QJsonParseError::NoError) {
        qDebug() << "parseJson:" << error->errorString();
        return false;
    }

    json = jdc.object();  //获取对象
    return true;
}

bool writeJson(const QString &filename, const QJsonObject &json) {
    //打开文件
    QFile file(QApplication::applicationDirPath() + "/" + filename);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open failed!";
        return false;
    }
    else {
        qDebug() << "File open successfully!";
    }
    QJsonDocument jdoc;
    jdoc.setObject(json);
    file.write(
        jdoc.toJson(QJsonDocument::Compact));  // Indented:表示自动添加/n回车符
    file.close();
    return false;
}

Game::Game(QPoint map_size, QObject *parent) : QObject(parent) {
    m_gameInfo.m_turnNumber = 0, m_gameInfo.map_size = map_size,
    m_gameInfo.nowPlayer = 0, m_gameInfo.playerNumbers = 2;
    nextTurnButtonWidget = nullptr, newUnitButtonWidget = nullptr,
    pauseButtonWidget = nullptr, policyTreeButtonWidget = nullptr;
    m_blocks.resize(width() + 2);

    m_unitTypeInfo[studentUnit] = UnitInfo("学生", "学生", 15, 1, 4);
    m_unitTypeInfo[teacherUnit] = UnitInfo("教师", "教师", 20, 2, 3);
    m_unitTypeInfo[childUnit]   = UnitInfo("孩子", "孩子", 10, 3, 2);
    m_unitTypeInfo[alphaUnit]   = UnitInfo("Alpha 病毒", "Alpha", 5, 2, 2);
    m_unitTypeInfo[deltaUnit]   = UnitInfo("Delta 病毒", "Delta", 5, 1, 5);
    m_unitTypeInfo[zetaUnit]    = UnitInfo("Zeta 病毒", "Boss", 15, 3, 3);

    m_blockTypeInfo[plainBlock]     = BlockInfo("平地", "平地", 0, 2);
    m_blockTypeInfo[obstacleBlock]  = BlockInfo("障碍", "障碍", 0, 10000);
    m_blockTypeInfo[roadBlock]      = BlockInfo("平地", "平地", 0, 1);
    m_blockTypeInfo[dampBlock]      = BlockInfo("泥地", "平地", 0, 3);
    m_blockTypeInfo[virusCampBlock] = BlockInfo("人类营地", "平地", 30, 2);
    m_blockTypeInfo[peopleCampBlock] = BlockInfo("病毒营地", "平地", 30, 2);

    for(int i = 1; i <= width(); i++) {
        m_blocks[i].resize(height() + 2);
        for(int j = 1; j <= height(); j++) {
            int t = QRandomGenerator::global()->generate() % 4;
            m_blocks[i][j] =
                new BlockStatus(t ? plainBlock : obstacleBlock,
                                t ? &m_blockTypeInfo[plainBlock] :
                                    &m_blockTypeInfo[obstacleBlock],
                                QPoint(i, j));
        }
    }
    m_field = new Field(m_gameInfo, m_blocks, m_units);
    m_graph = new GraphField(m_gameInfo, m_blocks, m_units);
    m_view  = new GraphView;
    m_view->setScene(m_graph);

    connect(m_view, &GraphView::finishPainting, this, &Game::setFixedWidgetPos);

    connect(m_graph, &GraphField::checkStateChange, this,
            [=](QPoint, bool state) {
                if(state == true) {
                    showNewUnitButton();
                }
                else {
                    hideNewUnitButton();
                }
            });
    usernextTurn();
}

Game::~Game() {
    clearMemory();
}

void Game::clearMemory() {
    for(int i = 1; i <= width(); i++) {
        for(int j = 1; j <= height(); j++) {
            delete m_blocks[i][j];
        }
    }
    for(int i = 0; i < m_units.size(); i++) {
        delete m_units[i];
    }
}

Game::Game(const QString &filename)
    : Game([](const QString &filename) -> QJsonObject {
          QJsonObject json;
          openJson(filename, json);
          return json;
      }(filename)) {}

Game::Game(const qint32 &level)
    : Game([](qint32 level) -> QString {
          return "json/level" + QString::number(level) + ".json";
      }(level)) {}

Game::Game(const QJsonObject &json) {
    // 本质上是一个 read 函数
    if(json.contains("gameInfo") && json["gameInfo"].isObject()) {
        m_gameInfo.read(json["gameInfo"].toObject());
    }
    if(json.contains("unitTypeInfo") && json["unitTypeInfo"].isObject()) {
        QJsonObject unitTypeInfo = json["unitTypeInfo"].toObject();
        for(auto it = unitTypeInfo.begin(); it != unitTypeInfo.end(); it++) {
            QJsonObject unitInfo = it.value().toObject();
            UnitInfo    _unitInfo;
            _unitInfo.read(unitInfo);
            m_unitTypeInfo[it.key().toInt()] = _unitInfo;
        }
    }
    if(json.contains("blockTypeInfo") && json["blockTypeInfo"].isObject()) {
        QJsonObject blockTypeInfo = json["blockTypeInfo"].toObject();
        for(auto it = blockTypeInfo.begin(); it != blockTypeInfo.end(); it++) {
            QJsonObject blockInfo = it.value().toObject();
            BlockInfo   _blockInfo;
            _blockInfo.read(blockInfo);
            m_blockTypeInfo[it.key().toInt()] = _blockInfo;
        }
    }
    if(json.contains("blocks") && json["blocks"].isArray()) {
        QJsonArray blocks = json["blocks"].toArray();
        int        cnt    = 0;
        m_blocks.clear();
        m_blocks.resize(width() + 2);
        for(int i = 1; i <= width(); i++) {
            m_blocks[i].resize(height() + 2);
            for(int j = 1; j <= height(); j++) {
                m_blocks[i][j] = new BlockStatus();
                m_blocks[i][j]->read(blocks[cnt].toObject());
                m_blocks[i][j]->m_info =
                    &m_blockTypeInfo[m_blocks[i][j]->m_type];
                cnt++;
            }
        }
    }
    if(json.contains("units") && json["units"].isArray()) {
        QJsonArray units = json["units"].toArray();
        m_units.clear();
        m_units.resize(units.size());
        for(int i = 0; i < units.size(); i++) {
            m_units[i] = new UnitStatus();
            m_units[i]->read(units[i].toObject());
            m_units[i]->m_info = &m_unitTypeInfo[m_units[i]->m_type];
        }
    }
    m_field = new Field(m_gameInfo, m_blocks, m_units);
    m_graph = new GraphField(m_gameInfo, m_blocks, m_units);
    m_view  = new GraphView;
    m_view->setScene(m_graph);
    connect(m_view, &GraphView::finishPainting, this, &Game::setFixedWidgetPos);

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

void Game::write(QJsonObject &json) {
    QJsonObject gameInfo;
    m_gameInfo.write(gameInfo);
    json["gameInfo"] = gameInfo;
    {
        QJsonObject unitTypeInfo;
        for(auto it = m_unitTypeInfo.begin(); it != m_unitTypeInfo.end();
            it++) {
            QJsonObject unitInfo;
            it.value().write(unitInfo);
            unitTypeInfo[QString::number(it.key())] = unitInfo;
        }
        json["unitTypeInfo"] = unitTypeInfo;
    }
    {
        QJsonObject blockTypeInfo;
        for(auto it = m_blockTypeInfo.begin(); it != m_blockTypeInfo.end();
            it++) {
            QJsonObject blockInfo;
            it.value().write(blockInfo);
            blockTypeInfo[QString::number(it.key())] = blockInfo;
        }
        json["blockTypeInfo"] = blockTypeInfo;
    }
    {
        QJsonArray blocks;
        for(int i = 1; i <= width(); i++) {
            for(int j = 1; j <= height(); j++) {
                QJsonObject block;
                m_blocks[i][j]->write(block);
                blocks.append(block);
            }
        }
        json["blocks"] = blocks;
    }
    {
        QJsonArray units;
        for(int i = 0; i < m_units.size(); i++) {
            QJsonObject unit;
            m_units[i]->write(unit);
            units.append(unit);
        }
        json["units"] = units;
    }
}

void Game::setgameStatusLabel() {
    // 应该重载一下那个 Label 的，不过之后再说吧，现在先写一个能用的
    QLabel *gameStatusLabel = new QLabel();
    gameStatusLabel->setStyleSheet(
        "font-size: 40px; background-color: rgba(244,244,244,100)");
    connect(this, &Game::gameStatusUpdated, this,
            [=]() { this->updateGameStatus(gameStatusLabel); });
    gameStatusLabelWidget = m_graph->addWidget(gameStatusLabel);
    gameStatusLabelWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                   true);
    this->updateGameStatus(gameStatusLabel);
}

void Game::updateGameStatus(QLabel *gameStatusLabel) {
    gameStatusLabel->setText(
        "回合数：" + QString::number(m_gameInfo.m_turnNumber) +
        " 现在正在操作的玩家：" + QString::number(m_gameInfo.nowPlayer));
}

void Game::setFixedWidgetPos() {
    if(nextTurnButtonWidget != nullptr) {
        nextTurnButtonWidget->setPos(m_view->mapToScene(
            QPoint(m_view->size().width(), m_view->height()) -
            QPoint(nextTurnButtonWidget->size().width(),
                   nextTurnButtonWidget->size().height())));
        nextTurnButtonWidget->setZValue(100);
    }
    if(newUnitButtonWidget != nullptr) {
        newUnitButtonWidget->setPos(m_view->mapToScene(
            QPoint(m_view->size().width() / 2 -
                       newUnitButtonWidget->size().width() / 2,
                   m_view->height() - newUnitButtonWidget->size().height())));
        newUnitButtonWidget->setZValue(100);
    }
    if(pauseButtonWidget != nullptr) {
        pauseButtonWidget->setPos(
            m_view->mapToScene(QPoint(m_view->size().width() - 100, 0)));
        pauseButtonWidget->setZValue(100);
    }
    if(policyTreeButtonWidget != nullptr) {
        policyTreeButtonWidget->setPos(
            m_view->mapToScene(QPoint(0, m_view->size().height() - 100)));
        policyTreeButtonWidget->setZValue(100);
    }
    if(gameStatusLabelWidget != nullptr) {
        gameStatusLabelWidget->setPos(m_view->mapToScene(
            QPoint(m_view->size().width() / 2 -
                       gameStatusLabelWidget->size().width() / 2,
                   0)));
        gameStatusLabelWidget->setZValue(100);
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
    QJsonObject json;
    write(json);
    writeJson("3.json", json);
    if(m_gameInfo.playerNumbers == m_gameInfo.nowPlayer) {
        m_gameInfo.nowPlayer = 1;
        m_gameInfo.m_turnNumber++;
    }
    else {
        m_gameInfo.nowPlayer += 1;
    }
    for(int i = 0; i < m_units.size(); i++) {
        if(m_units[i]->m_player == m_gameInfo.nowPlayer) {
            m_units[i]->setAttackState(true);
            m_units[i]->setMoveState(true);
        }
        else {
            m_units[i]->setAttackState(false);
            m_units[i]->setMoveState(false);
        }
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
    // 初始时需要隐藏
    newUnitButtonWidget->hide();

    connect(newUnitButton, &QPushButton::clicked, this, &Game::usernewUnit);
}

void Game::usernewUnit() {
    // 需要当前位置没有Unit，否则会炸掉的
    NewUnitDialog *newunit     = new NewUnitDialog(this);
    int            newUnitType = newunit->exec();
    if(newUnitType == 0)
        return;
    delete newunit;
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
        m_units.size(), UnitType(newUnitType), &m_unitTypeInfo[newUnitType],
        m_gameInfo.nowPlayer, m_graph->m_nowCheckedBlock->coord());

    emit m_graph->checkStateChange(m_graph->m_nowCheckedBlock->coord(), false);
    m_graph->m_nowCheckedBlock = nullptr;
    m_units.push_back(unitStatus);
    m_field->doNewUnit(unitStatus);
}

void Game::setPolicyTreeButton() {
    QPushButton *policyTreeButton = new QPushButton();
    policyTreeButton->setGeometry(QRect(0, -100, 100, 100));
    policyTreeButton->setIcon(QIcon(":/icons/policytree.png"));
    policyTreeButton->setIconSize(QSize(85, 85));
    policyTreeButton->setContentsMargins(5, 5, 5, 5);
    policyTreeButtonWidget = m_graph->addWidget(policyTreeButton);
    policyTreeButtonWidget->setFlag(QGraphicsItem::ItemIgnoresTransformations,
                                    true);
    connect(policyTreeButton, &QPushButton::clicked, this,
            &Game::usershowPolicyTree);
}

void Game::usershowPolicyTree() {
    PolicyTreeDialog t(this, nullptr);
    t.exec();
}
