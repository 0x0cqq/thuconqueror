#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class StartWindow;
class ChooseLevelWindow;
}  // namespace Ui

class StartWindow : public QMainWindow {
    Q_OBJECT
  public:
    Ui::StartWindow *      startview_ui;
    Ui::ChooseLevelWindow *chooselevelview_ui;

    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    void addLevelInfo(int level);
    // bool eventFilter(QObject *object, QEvent *event);
  signals:
    void userChooseLevel(int level);
  public slots:
    void changeToChooseLevelWindow();
};

#endif