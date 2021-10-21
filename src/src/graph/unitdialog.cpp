#include "unitdialog.h"
#include "ui_unitdialog.h"
#include <QLabel>
#include <QLayout>

UnitDialog::UnitDialog(const UnitStatus *unitStatus, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint), ui(nullptr),
      m_status(unitStatus), showTimes(0) {
    ui = new Ui::UnitDialog;
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    // this->layout()->addWidget(detailLabel);
}

void UnitDialog::updateInfo() {
    // qDebug() << m_status->m_info->image << Qt::endl;
    ui->name->setText(m_status->m_info->name);
    ui->noLabel->setText("No. " + QString::number(uid()));
    ui->hpLabel->setText("血量：" +
                         QString::number(m_status->getHP(), char(103), 2));
    ui->mpLabel->setText("移动力：" + QString::number(m_status->getMP()));
    ui->ceLabel->setText("攻击力：" + QString::number(m_status->getCE()));
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

AttackLabel::AttackLabel(qreal delta, QWidget *parent)
    : QLabel(parent), timer(nullptr) {
    this->setText("-" + QString::number(delta));
    this->setFont(QFont("微软雅黑", 15, 3));
    timer = new QTimer(this);
}

void AttackLabel::show() {
    // qDebug() << "show label";
    QLabel::show();
    timer->singleShot(2000, [=]() { this->hide(); });
}
