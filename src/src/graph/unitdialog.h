#ifndef UNITDIALOG_H
#define UNITDIALOG_H

#include "../basic/status.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QTimer>

namespace Ui {
class UnitDialog;
}


// 跟显示Unit 信息有关的对话框

class UnitDialog : public QDialog {
    Q_OBJECT
  public:
    Ui::UnitDialog *  ui;
    const UnitStatus *m_status;
    QMovie *          m_movie;
    int               showTimes;
    UnitDialog(const UnitStatus *unitStatus, QWidget *parent);
    void show();
    void hide();
    void updateInfo();
};

class AttackLabel : public QLabel {
    Q_OBJECT
  public:
    QTimer *timer;
    AttackLabel(qreal delta, QWidget *parent);
    ~AttackLabel() {}
    void show();
};

#endif