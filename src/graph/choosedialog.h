#ifndef CHOOSEDIALOG_H
#define CHOOSEDIALOG_H

#include <QDialog>
#include <QString>
#include <QVector>

class LevelInfo {
  public:
    int     levelnumber;
    QString name;
    QString description;
};

class ChooseDialog : public QDialog {
    Q_OBJECT
  public:
    ChooseDialog(QVector<LevelInfo> levels, QWidget *parent = nullptr);
  signals:
    void userChooseLevel(int level);
};

#endif