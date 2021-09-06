#include "menudialog.h"
#include "../game.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>

MenuDialog::MenuDialog(Game *game, QWidget *parent)
    : QDialog(parent), m_game(game) {}

PauseMenuDialog::PauseMenuDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {}

PolicyTreeDialog::PolicyTreeDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this->layout()->addWidget(new QPushButton());
}

NewUnitDialog::NewUnitDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));

    // this->layout()->addWidget(new QPushButton());
    // 根据 UnitType 制造？Game 的指针可以用起来
    for(auto it = game->m_unitTypeInfo.begin(); it != game->m_unitTypeInfo.end();
        it++) {
        if((it.key() & (1 << game->m_gameInfo.nowPlayer)) == 0){
            continue;
        }
        QGroupBox *groupbox = new QGroupBox();
        groupbox->setLayout(new QVBoxLayout);
        groupbox->setTitle(it.value().name);
        groupbox->layout()->addWidget(new QLabel(it.value().description));
        groupbox->layout()->addWidget(new QLabel(
            "生命力: " +
            QString::number(it.value().HPfull * it.value().HPratio)));
        groupbox->layout()->addWidget(new QLabel(
            "攻击力: " +
            QString::number(it.value().CEfull * it.value().CEratio)));
        groupbox->layout()->addWidget(
            new QLabel("移动力: " + QString::number(it.value().MPfull)));
        QPushButton *pb = new QPushButton("新建单元");
        connect(pb, &QPushButton::clicked, this, [=]() { done(it.key()); });
        groupbox->layout()->addWidget(pb);
        this->layout()->addWidget(groupbox);
    }
}