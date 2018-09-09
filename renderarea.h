#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QColor>
#include "grid.h"

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void newGame(int size);
    bool isFinished();
    void undoMove();
    void updateMoves(int moves, int last); //send signal to update moves

private:
    QColor mBackgroundColor;
    QColor mBorderColor;
    QColor mPenColor;
    Grid tileGrid;
    int mMoves = 0;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent ( QMouseEvent * event ) override;
    void drawTileGrid();
    void drawBackground(QPainter* painter);
    void setDigitFont(QPainter* painter);
    void drawFinish(QPainter* painter);

signals:
  void  sendMoves( int moves, int last);

public slots:

};


#endif // RENDERAREA_H
