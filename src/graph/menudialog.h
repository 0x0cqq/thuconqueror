#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>

class Game;

class MenuDialog : public QDialog {
    Q_OBJECT
  protected:
    Game *m_game;
    MenuDialog(Game *game, QWidget *parent = nullptr);
};

class PauseMenuDialog : public MenuDialog {
    Q_OBJECT
  public:
    PauseMenuDialog(Game *game, QWidget *parent = nullptr);
};

class PolicyTreeDialog : public MenuDialog {
    Q_OBJECT
  public:
    PolicyTreeDialog(Game *game, QWidget *parent = nullptr);
};

class NewUnitDialog : public MenuDialog {
    Q_OBJECT
  public:
    NewUnitDialog(Game *game, QWidget *parent = nullptr);
};

#endif  // MENUDIALOG_H