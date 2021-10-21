#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>
#include <QString>

class Game;


// 一些乱七八糟的对话框

namespace Ui {
class SingleUnitInfo;
class PauseDialog;
class ChooseFileDialog;
};  // namespace Ui

class ChooseFileDialog : public QDialog {
    Q_OBJECT
  public:
    Ui::ChooseFileDialog *ui;
    ChooseFileDialog(QStringList filelist, QWidget *parent);
};

class MenuDialog : public QDialog {
    Q_OBJECT
  protected:
    Game *m_game;
    MenuDialog(Game *game, QWidget *parent);
};

class PauseMenuDialog : public MenuDialog {
    Q_OBJECT
  public:
    Ui::PauseDialog *ui;
    PauseMenuDialog(Game *game, QString &string, QWidget *parent);
    bool save();
};

class PolicyTreeDialog : public MenuDialog {
    Q_OBJECT
  public:
    PolicyTreeDialog(Game *game, QWidget *parent);
};

class SingleUnitInfo : public QWidget {
    Q_OBJECT
  public:
    Ui::SingleUnitInfo *ui;
    SingleUnitInfo(QWidget *parent);
};

class NewUnitDialog : public MenuDialog {
    Q_OBJECT
  public:
    NewUnitDialog(Game *game, QWidget *parent);
};

#endif  // MENUDIALOG_H