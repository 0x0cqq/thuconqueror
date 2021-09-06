#include "choosedialog.h"

#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>

ChooseDialog::ChooseDialog(QVector<LevelInfo> levels, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint) {
    this->setLayout(new QHBoxLayout);
    for(int i = 0; i < levels.size(); i++) {
        auto gb = new QGroupBox;
        gb->setFixedWidth(400);
        gb->setTitle("第 " + QString::number(levels[i].levelnumber) + " 关");
        gb->setLayout(new QVBoxLayout);
        auto nameLabel = new QLabel(levels[i].name);
        nameLabel->setWordWrap(true);
        gb->layout()->addWidget(nameLabel);
        dynamic_cast<QVBoxLayout *>(gb->layout())->addSpacing(100);
        dynamic_cast<QVBoxLayout *>(gb->layout())->addStretch();
        auto descriptionLabel = new QLabel(levels[i].description);
        descriptionLabel->setWordWrap(true);
        gb->layout()->addWidget(descriptionLabel);
        auto pb = new QPushButton;
        pb->setText("开始");
        gb->layout()->addWidget(pb);
        connect(pb, &QPushButton::clicked, this,
                [=]() { emit userChooseLevel(levels[i].levelnumber); });
        this->layout()->addWidget(gb);
    }
}