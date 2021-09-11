#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>
#include <QString>

class Game;

namespace Ui {
class SingleUnitInfo;
class PauseDialog;
class ChooseFileDialog;
};  // namespace Ui

class ChooseFileDialog : public QDialog {
    Q_OBJECT
  public:
    Ui::ChooseFileDialog *ui;
    ChooseFileDialog(QStringList filelist, QWidget *parent = nullptr);
};

class MenuDialog : public QDialog {
    Q_OBJECT
  protected:
    Game *m_game;
    MenuDialog(Game *game, QWidget *parent = nullptr);
};

class PauseMenuDialog : public MenuDialog {
    Q_OBJECT
  public:
    Ui::PauseDialog *ui;
    PauseMenuDialog(Game *game, QWidget *parent = nullptr);
    bool save();
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