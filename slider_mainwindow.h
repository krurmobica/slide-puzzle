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
    void updateMoves( int moves, int last );

    void on_undoBtn_clicked();

    void on_actionCheats_changed();

private:
    mutable int msec = 0;
    Ui::SliderMainWindow *ui;
    void startTimer();

    QString timeToString(int milliseconds){
        int seconds = milliseconds / 1000;
        milliseconds %= 1000;

        int minutes = seconds / 60;
        seconds %= 60;

        int hours = minutes / 60;
        minutes %= 60;

        QTime t(hours, minutes, seconds, milliseconds);

        return t.toString("hh:mm:ss.zzz");
    }
    QTimer *timer;
    bool timerStop = false;
};

#endif // SLIDER_MAINWINDOW_H
