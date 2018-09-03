#include "renderarea.h"

#include <iostream>
using namespace std;

#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mBackgroundColor (255,255,255),
    mBorderColor (0,0,0), //borders color
    mPenColor (0,0,0) //text color
{

}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100,100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400,200);
}

void RenderArea::mousePressEvent ( QMouseEvent * event )
{
    if (this->initialized == true){
        if (event->button() == Qt::LeftButton) {
            QPoint point = event->pos();
            move(point);
        }
    }
}

void RenderArea::move(QPoint point)
{
    if (initialized and !finished){
        if (point.x() < mGridSize * mTileSize && point.y() < mGridSize * mTileSize){  //out of game bounds after resizing will cause issues with 1D index access
            tile *t = getTile(point);
            if (t->val != 0){ //blank tile is not moved
                swap(t, 1);
                this->repaint();
            }
        }
    }
}

void  RenderArea:: updateMoves( int moves, int last ){
    //send signal to update movesBox
    emit sendMoves(moves, last);
}

void RenderArea:: paintEvent(QPaintEvent *event)
{
    if (this->initialized == true){
            drawTileGrid();
    } else {
        QPainter painter(this);
        painter.setBrush(mBackgroundColor);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(mBorderColor);
        painter.drawRect(this->rect());
    }
}

void RenderArea:: drawTileGrid()
{
    QPainter painter(this);
    painter.setBrush(mBackgroundColor);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(mBorderColor);

    QFont font = painter.font();
    font.setPointSize((int)(mTileSize/(3+floor(mGridSize/10)))); //Adjust font size
    painter.setFont(font);

    for( int i=0; i < mGridSize*mGridSize; i++ )
    {
        painter.setPen(mBorderColor);
        tile t = (gridElements[i].tile);
        painter.drawRect(t.rec);
        painter.setPen(mPenColor);
        if (t.val > 0) {painter.drawText(t.rec, Qt::AlignCenter, QString("%1").arg(t.val));}
    }
    if (finished){
        font.setPointSize((int)((mTileSize*mGridSize)/8)); //Adjust font size
        painter.setFont(font);
        painter.setPen(QColor("blue"));
        painter.drawText(0,0,mTileSize*mGridSize,mTileSize*mGridSize, Qt::AlignCenter, QString("FINISHED"));
    }
}

