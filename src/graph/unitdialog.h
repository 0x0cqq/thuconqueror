#ifndef UNITDIALOG_H
#define UNITDIALOG_H

#include "../basic/status.h"
#include <QDialog>
#include <QLabel>

class UnitDialog : public QDialog {
    Q_OBJECT
  public:
    const UnitStatus *m_status;
    QLabel *    detailLabel;
    int showTimes;
    UnitDialog(const UnitStatus *unitStatus, QWidget *parent = nullptr);
    void show();
    void hide();
    void updateInfo();
};

#endif