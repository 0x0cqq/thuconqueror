#include "menudialog.h"
#include "../game.h"

#include <QBoxLayout>
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
