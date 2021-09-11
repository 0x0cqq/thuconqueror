#include "menudialog.h"
#include "../basic/status.h"
#include "../game/game.h"
#include "ui_choosefiledialog.h"
#include "ui_newunitwidget.h"
#include "ui_pausedialog.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>

ChooseFileDialog::ChooseFileDialog(QStringList filelist, QWidget *parent)
    : QDialog(parent), ui(nullptr) {
    ui = new Ui::ChooseFileDialog();
    ui->setupUi(this);
    ui->comboBox->addItems(filelist);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, [=]() { done(ui->comboBox->currentIndex()); });
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel),
            &QPushButton::clicked, this, [=]() { done(-1); });
}

MenuDialog::MenuDialog(Game *game, QWidget *parent)
    : QDialog(parent), m_game(game) {}

// 第 i 关采用 `level1-202001012059.json` 的形式存储
QStringList getFileNames(int i, const QString &path) {
    QDir        dir(path);
    QStringList nameFilters;
    qDebug() << i << " path" << path;
    nameFilters << "level" + QString::number(i) + "-*.json";
    QStringList files =
        dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for(auto it = files.begin(); it != files.end(); it++) {
        *it = path + "/" + (*it);
    }
    return files;
}

PauseMenuDialog::PauseMenuDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent), ui(nullptr) {
    ui = new Ui::PauseDialog;
    ui->setupUi(this);
    connect(this->ui->readButton, &QPushButton::clicked, this, [=]() {
        ChooseFileDialog *dia = new ChooseFileDialog(
            getFileNames(game->m_gameInfo.level,
                         QApplication::applicationDirPath() + "/json"),
            this);
        int t = dia->exec();
        qDebug() << "actually opened" << t << " "
                 << getFileNames(game->m_gameInfo.level,
                                 QApplication::applicationDirPath() +
                                     "/json")[t];
        QJsonObject json;
        openJsonAbsPath(
            getFileNames(game->m_gameInfo.level,
                         QApplication::applicationDirPath() + "/json")[t],
            json);
        game->clearConnection();
        game->clearMemory();
        game->read(json);
        game->init();
        QMessageBox::information(this, "yes!", "加载成功！");
        dia->deleteLater();
        done(0);
    });
    connect(this->ui->saveButton, &QPushButton::clicked, this, [=]() {
        save();
        QMessageBox::information(this, "yes!", "保存成功！");

        done(0);
    });
    connect(this->ui->exitButton, &QPushButton::clicked, this,
            [=] { done(1); });
}

bool PauseMenuDialog::save() {
    QJsonObject json;
    m_game->write(json);
    QDateTime time  = QDateTime::currentDateTime();  //获取当前时间
    int       timeT = time.toTime_t();  //将当前时间转为时间戳

    writeJson("json/level" + QString::number(m_game->m_gameInfo.level) + "-" +
                  QString::number(timeT) + ".json",
              json);
    return true;
}

PolicyTreeDialog::PolicyTreeDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this->layout()->addWidget(new QPushButton());
}
SingleUnitInfo::SingleUnitInfo(QWidget *parent) : QWidget(parent), ui(nullptr) {
    ui = new Ui::SingleUnitInfo();
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
}

NewUnitDialog::NewUnitDialog(Game *game, QWidget *parent)
    : MenuDialog(game, parent) {
    this->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));

    // this->layout()->addWidget(new QPushButton());
    // 根据 UnitType 制造？Game 的指针可以用起来
    for(auto it = game->m_unitTypeInfo.begin();
        it != game->m_unitTypeInfo.end(); it++) {
        if((it.key() & (1 << game->m_gameInfo.nowPlayer)) == 0) {
            continue;
        }
        QMovie *movie = new QMovie(it.value().image, QByteArray(), this);
        movie->start();
        SingleUnitInfo *widget = new SingleUnitInfo;
        widget->ui->image->setMovie(movie);
        widget->ui->name->setText(it.value().name);
        widget->ui->description->setText(it.value().description);
        widget->ui->hpLabel->setText(
            "生命力: " +
            QString::number(it.value().HPfull * it.value().HPratio));
        widget->ui->ceLabel->setText(
            "攻击力: " +
            QString::number(it.value().CEfull * it.value().CEratio));
        widget->ui->mpLabel->setText(
            ("移动力: " + QString::number(it.value().MPfull)));
        connect(widget->ui->newButton, &QPushButton::clicked, this,
                [=]() { done(it.key()); });
        this->layout()->addWidget(widget);
    }
}