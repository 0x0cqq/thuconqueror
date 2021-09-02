#include "game.h"
#include <QRandomGenerator>

Game::Game(GraphView *graphView, QPoint map_size, QObject *parent)
    : QObject(parent) {
    m_gameInfo.m_turnNumber = 0, m_gameInfo.map_size = map_size,
    m_gameInfo.nowPlayer = 1, m_gameInfo.playerNumbers = 2;
    m_blocks.resize(width() + 2);
    for(int i = 1; i <= width(); i++) {
        m_blocks[i].resize(height() + 2);
        for(int j = 1; j <= height(); j++) {
            m_blocks[i][j] = new BlockStatus(QRandomGenerator::global()->generate() % 4 != 0 ? plainBlock : obstacleBlock , QPoint(i, j));
        }
    }
    m_field = new Field(m_gameInfo, m_blocks, m_units);
    m_graph = new GraphField(m_gameInfo, m_blocks, m_units);
    if(graphView != nullptr) {
        graphView->setScene(m_graph);
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
    msgBox.setText("进入下一玩家游戏。当前是第 " + QString::number(m_gameInfo.m_turnNumber) + " 回合，第 " + QString::number(m_gameInfo.nowPlayer) + " 号玩家，请开始操控。");
    msgBox.exec();
    emit gameStatusUpdated();
}