#ifndef UNITDIALOG_H
#define UNITDIALOG_H

#include "../basic/status.h"
#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QTimer>

class UnitDialog : public QDialog {
    Q_OBJECT
  public:
    const UnitStatus *m_status;
    QLabel *          detailLabel;
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