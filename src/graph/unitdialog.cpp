#include "unitdialog.h"
#include <QLabel>
#include <QLayout>

UnitDialog::UnitDialog(UnitStatus *unitStatus, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint), m_status(unitStatus),
      detailLabel(nullptr), showTimes(0) {
    detailLabel = new QLabel(this);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(detailLabel);
}

void UnitDialog::updateInfo() {
    QString text;
    text.append("Unit 编号：" + QString::number(uid()) + "\n");
    text.append("所属玩家：" + QString::number(player()) + "\n");
    text.append("血量：" + QString::number(m_status->getHP()) + "\n移动力：" +
                QString::number(m_status->getMP()) + "\n攻击力：" +
                QString::number(m_status->getCE()));
    detailLabel->setText(text);
}

void UnitDialog::show() {
    updateInfo();
    showTimes++;
    if(showTimes >= 1) {
        QDialog::show();
    }
}

void UnitDialog::hide() {
    // updateInfo();
    showTimes--;
    if(showTimes <= 0) {
        QDialog::hide();
    }
}