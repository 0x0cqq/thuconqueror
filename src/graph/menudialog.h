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

namespace Ui {
class SingleUnitInfo;
class PauseDialog;
};  // namespace Ui

class PauseMenuDialog : public MenuDialog {
    Q_OBJECT
  public:
    Ui::PauseDialog *ui;
    PauseMenuDialog(Game *game, QWidget *parent = nullptr);
};

class PolicyTreeDialog : public MenuDialog {
    Q_OBJECT
  public:
    PolicyTreeDialog(Game *game, QWidget *parent = nullptr);
};

class SingleUnitInfo : public QWidget {
    Q_OBJECT
  public:
    Ui::SingleUnitInfo *ui;
    SingleUnitInfo(QWidget *parent = nullptr);
};

class NewUnitDialog : public MenuDialog {
    Q_OBJECT
  public:
    NewUnitDialog(Game *game, QWidget *parent = nullptr);
};

#endif  // MENUDIALOG_H