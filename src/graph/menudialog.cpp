#include "menudialog.h"
#include "../game/game.h"
#include "ui_newunitwidget.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>

MenuDialog::MenuDialog(Game *game, QWidget *parent)
    : QDialog(parent), m_game(game) {}

PauseMenuDialog::PauseMenuDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent), ui(nullptr) {}

PolicyTreeDialog::PolicyTreeDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this->layout()->addWidget(new QPushButton());
}
SingleUnitInfo::SingleUnitInfo(QWidget *parent) : ui(nullptr) {
    ui = new Ui::SingleUnitInfo();
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
}

NewUnitDialog::NewUnitDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));

    // this->layout()->addWidget(new QPushButton());
    // 根据 UnitType 制造？Game 的指针可以用起来
    for(auto it = game->m_unitTypeInfo.begin();
        it != game->m_unitTypeInfo.end(); it++) {
        if((it.key() & (1 << game->m_gameInfo.nowPlayer)) == 0) {
            continue;
        }
        QMovie *movie = new QMovie(it.value().image, QByteArray(), this);
        movie->start();
        SingleUnitInfo *widget = new SingleUnitInfo;
        widget->ui->image->setMovie(movie);
        widget->ui->name->setText(it.value().name);
        widget->ui->description->setText(it.value().description);
        widget->ui->hpLabel->setText(
            "生命力: " +
            QString::number(it.value().HPfull * it.value().HPratio));
        widget->ui->ceLabel->setText(
            "攻击力: " +
            QString::number(it.value().CEfull * it.value().CEratio));
        widget->ui->mpLabel->setText(
            ("移动力: " + QString::number(it.value().MPfull)));
        connect(widget->ui->newButton, &QPushButton::clicked, this,
                [=]() { done(it.key()); });
        this->layout()->addWidget(widget);
    }
}