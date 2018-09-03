#ifndef SLIDER_MAINWINDOW_H
#define SLIDER_MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

namespace Ui {
class SliderMainWindow;
}

class SliderMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SliderMainWindow(QWidget *parent = nullptr);
    ~SliderMainWindow();


private slots:
    void on_newgameBtn_clicked();    
    void on_undoBtn_clicked();
    void on_actionCheats_changed();
    void updateMoves( int moves, int last );

private:
    mutable int msec = 0;
    Ui::SliderMainWindow *ui;
    void startTimer();
    QString timeToString(int milliseconds);
    QTimer *timer;
    bool timerStop = false;
};

#endif // SLIDER_MAINWINDOW_H
