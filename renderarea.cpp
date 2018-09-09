#include "renderarea.h"
#include "tile.h"

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
    mPenColor (0,0,0), //text color
    tileGrid()
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

void RenderArea::newGame(int size){
    mMoves = 0;
    tileGrid.newGrid(size, this->rect());
}

void RenderArea::mousePressEvent ( QMouseEvent * event )
{
    if (tileGrid.getSize() > 0 && !tileGrid.isFinished()){
        if (event->button() == Qt::LeftButton) {
            QPoint point = event->pos();
            if (point.x() > tileGrid.getXOffset() && point.x() < rect().width()-tileGrid.getXOffset()){
                Tile* clicked_tile = tileGrid.getTile(point);
                if(clicked_tile->isMovable()){
                    int lastVal = clicked_tile->getValue();
                    tileGrid.move(clicked_tile);
                    this->repaint();
                    mMoves += 1;
                    updateMoves(mMoves, lastVal);
                }
            }
        }
    }
}

void RenderArea:: paintEvent(QPaintEvent *event)
{
    if (tileGrid.getSize() > 0){
        tileGrid.resize(this->rect());
        drawTileGrid();
    } else {
        QPainter painter(this);
        drawBackground(&painter);
    }
}

void RenderArea::drawBackground(QPainter* painter){
    painter->setBrush(mBackgroundColor);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(mBorderColor);
    painter->drawRect(this->rect());
}

void RenderArea:: drawTileGrid()
{    
    QPainter painter(this);
    painter.setBrush(mBackgroundColor);
    painter.setPen(mBorderColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    setDigitFont(&painter);
    Tile* tileArray = tileGrid.getTileArray();

    for( int i=0; i < tileGrid.getMaxSize(); i++ )
    {
        Tile t = (tileArray[i]);
        t.draw(&painter, &mBorderColor, &mPenColor);
    }
    if (tileGrid.isFinished()) drawFinish(&painter);
}

void RenderArea::setDigitFont(QPainter* painter){
    QFont font = painter->font();
    font.setPointSize((int)(tileGrid.getTileSize()/(3+floor(tileGrid.getSize()/10)))); //Adjust font size
    painter->setFont(font);
}

void RenderArea::drawFinish(QPainter* painter){
    QFont font = painter->font();
    font.setPointSize((int)((tileGrid.getWidth())/8));
    painter->setFont(font);
    painter->setPen(QColor("blue"));
    painter->drawText(tileGrid.getXOffset(),0,tileGrid.getWidth(),tileGrid.getWidth(), Qt::AlignCenter, QString("FINISHED"));
}

void RenderArea::undoMove(){
    if (tileGrid.getSize() > 0){
        int last = tileGrid.getLast();
        if (last !=0){
            tileGrid.undo();
            mMoves -= 1;
            updateMoves(mMoves, last);
            this->repaint();
        }
    }
}

void  RenderArea:: updateMoves( int moves, int last ){
    //send signal to update movesBox
    emit sendMoves(moves, last);
}

bool RenderArea::isFinished(){
    return tileGrid.isFinished();
}
