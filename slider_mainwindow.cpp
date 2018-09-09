#include "slider_mainwindow.h"
#include "ui_slider_mainwindow.h"

#include <QMessageBox>
#include <QTimer>


SliderMainWindow::SliderMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SliderMainWindow)
{
    ui->setupUi(this);
    QObject::connect(this->ui->renderArea, SIGNAL(sendMoves(int, int)), this, SLOT(updateMoves(int, int)));
    this->setWindowTitle("Slide Puzzle Game");
}

SliderMainWindow::~SliderMainWindow()
{
    delete ui;
}

void SliderMainWindow::on_newgameBtn_clicked()
{
    int gridSize = this->ui->sizeComboBox->currentText().toInt();
    this->ui->renderArea->newGame(gridSize);

    updateMoves(0,0);
    this->ui->renderArea->repaint();
    startTimer();
}

void  SliderMainWindow::updateMoves( int moves, int last )
{
    QString strMoves = "";
    strMoves.setNum(moves);
    this->ui->movesField->setText(strMoves);
    QString strLast = "";
    strLast.setNum(last);
    this->ui->lastMove->setText(strLast);
    if (this->ui->renderArea->isFinished()){
        timerStop = true;

        QMessageBox msgBox;
        QString endTime = this->ui->timeBox->text();
        msgBox.setText(QString("Congratulations you have finished the game in %1 with %2 moves.").arg(endTime,strMoves));
        msgBox.exec();
    }
}

void SliderMainWindow::on_undoBtn_clicked()
{
    this->ui->renderArea->undoMove();
}

void SliderMainWindow::startTimer()
{
    msec = 0;
    timerStop = false;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,[=]() {
        msec+=10;
        if (timerStop){
            timer->stop();
        } else {
            int ms = msec;
            this->ui->timeBox->setText(timeToString(ms));
        }
    });
    timer->start(10);
}

QString SliderMainWindow::timeToString(int milliseconds){
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    int hours = minutes / 60;
    minutes %= 60;

    QTime t(hours, minutes, seconds, milliseconds);
    QString ret = t.toString("mm:ss.zzz");
    if (hours > 0) ret = t.toString("hh:mm:ss.zzz");
    return ret;
}
