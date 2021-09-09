#ifndef UNITDIALOG_H
#define UNITDIALOG_H

#include "../basic/status.h"
#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QMovie>
#include <QTimer>

namespace Ui {class UnitDialog; }

class UnitDialog : public QDialog {
    Q_OBJECT
  public:
    Ui::UnitDialog *ui;
    const UnitStatus *m_status;
    QMovie * m_movie;
    int               showTimes;
    UnitDialog(const UnitStatus *unitStatus, QWidget *parent = nullptr);
    void show();
    void hide();
    void updateInfo();
};

class AttackLabel : public QLabel {
    Q_OBJECT
  public:
    QTimer *timer;
    AttackLabel(qreal delta);
    ~AttackLabel() { delete timer; }
    void show();
};

#endif