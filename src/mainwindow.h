#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphblock.h"
#include "graphfield.h"
#include "graphview.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class Mainwindow : public QMainWindow {
    Q_OBJECT

  public:
    Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();

  private:
    QHBoxLayout *hBox;
    QVBoxLayout *hvBox;
    QHBoxLayout *buttonBox;
    QPushButton *exitButton;
    QPushButton *saveButton;
    QLabel *     detailLabel;
    QLabel *     policyTreeLabel;
    QPushButton *nextTurnButton;
    GraphView *  graphView;
    GraphField * graphField;
};
#endif  // MAINWINDOW_H
