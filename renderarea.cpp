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

void RenderArea::solvableGrid()
{
    int size = mGridSize;
    int maxValue = size*size;
    this->mMoves = 0;
    this->mGridArraySize = maxValue;
    this->gridElements = new gridElement[maxValue]; //use 1d array with size of size^2
    //calculate grid sizes with rects etc
    int tileSize;
    int x = this->rect().x();
    int y = this->rect().y();
    int h = this->rect().height();
    int w = this->rect().width();
    int d = 1;

    tileSize = (int)floor(h/size);

    if (w < h) tileSize = (int)floor(w/size);

    this->mTileSize = tileSize;

    vector<int> used;
    int counter = 1;
    for( int i=0; i < size; i++ )
    {

        for( int j=0; j < size; j++ )
        {
            int n;
            //n = predefined[counter-1];
            if (counter < maxValue){
                n = counter;
            } else {
                n = 0;
            }

            used.push_back(n);
            counter++;

            QRect r = QRect(x,y,tileSize,tileSize);
            tile t = {n, r, NULL, NULL, NULL, NULL};

            gridElement ge = {r, t};
            int index = j+i*size;
            this->gridElements[index] = ge; //offset as replacement to 2d array
            bindTile(index);
            if (n == 0) {
                this->blankTile = &this->gridElements[index].tile;
                this->lastTile = this->blankTile;
            }
            x+=tileSize;
        }
        x = this->rect().topLeft().x(); //reset x coord
        y+=tileSize;
    }

    randomize();

    initialized = true;
    finished = false;
}

void RenderArea::randomize(){
    int moves = 10;
    moves = rand()%((mGridSize*mGridSize)/2)+moves+mGridSize/2; //number form moves for different sizes
    node *lastNode = new node;
    lastNode->val = 0;
    for (int i = 0; i < moves; i++){
        tile *next;
        int direction;
        do {
             direction = rand()%4;
             next = getDirection(direction, blankTile);
        }
        while(next == NULL);
        int tileValue = next->val;
        if (tileValue != lastNode->val){
            node *n = new node;
            n->val = tileValue;
            n->tile = blankTile;
            n->last = lastNode;
            lastNode = n;

        } else { //ignore back forth moves
            if (lastNode->last != NULL){
                lastNode = lastNode->last;
                i--;
            } else {
                lastNode = new node;
                lastNode->val = 0;
            }
        }

        swapHidden(next);
    }

    this->lastNode = lastNode;
}



bool RenderArea::isFinished(){
    //check if game is finished
    if (gridElements[mGridArraySize-1].tile.val == 0){ // blank tile is in correct place
        for (int i = 0; i<mGridArraySize-1; i++){
            if (gridElements[i].tile.val != i+1) return false;
        }
        return true;
    }
    return false;
}

void RenderArea::undoMove(){
    //remember all moves
    swap(lastNode->tile, -1);
    if (lastNode->last->val != 0){
        lastNode = lastNode->last;
    } else {
        lastNode = new node;
    }
    this->repaint();
}

void RenderArea::bindTile(int index){
    //bind neighboring tiles together
    if (index > 0) //nothing to bind and most left
    {
        tile *thisTile = &gridElements[index].tile;
        if (index < mGridSize || index % (mGridSize) != 0) // first in new row
        {
            int indexLeft = index - 1; //left Node
            tile *leftTile = &gridElements[indexLeft].tile;
            thisTile->left = leftTile;
            leftTile->right = thisTile;
        }
        if (index >= mGridSize) //next rows
        {
            int indexUp = index - mGridSize;
            tile *upTile = &gridElements[indexUp].tile;
            thisTile->up = upTile;
            upTile->down = thisTile;
        }
    }
}

tile* RenderArea:: getDirection(int d, tile *t){
    switch(d){
        case 0: return t->up;
        case 1: return t->down;
        case 2: return t->left;
        case 3: return t->right;
    }
    return 0;
}

int RenderArea::getIndex(int x, int y)
{
    return (int)(floor(x/this->mTileSize) + mGridSize*floor(y/this->mTileSize));
}

int RenderArea::getIndex(tile *t)
{
    int x = t->rec.x();
    int y = t->rec.y();
    return (int)(floor(x/this->mTileSize) + mGridSize*floor(y/this->mTileSize));
}

tile* RenderArea:: getTile(QPoint point){
    int x = point.x();
    int y = point.y();
    int index = getIndex(x,y);
    if (index < this->mGridArraySize){ //out of bounds
        return &this->gridElements[index].tile;
    }
    return this->blankTile; //return blank tile as a false
}

void RenderArea::swap(tile *t, int move)
{
    if (cheats == true || isTileMovable(t)){
        //swap tile values with blank
        int l = t->val;
        if (move > 0)
        {
            int tileValue = t->val;
            node *n = new node;
            n->val = tileValue;
            n->tile = blankTile;
            n->last = this->lastNode;
            this->lastNode = n;
        }

        tile *lastTemp = this->blankTile;
        this->blankTile->val = t->val;
        t->val = 0;
        this->blankTile = t; //assign blank tile
        this->lastTile = lastTemp;

        //send update signal
        mMoves += move;
        updateMoves(mMoves, l);
    }
}

void RenderArea::swapHidden(tile *t)
{
    this->blankTile->val = t->val;
    t->val = 0;
    this->blankTile = t;
}

bool RenderArea::isTileMovable(tile *t){
    if (t->up != NULL && t->up->val == 0) return true;
    if (t->down != NULL && t->down->val == 0) return true;
    if (t->left != NULL && t->left->val == 0) return true;
    if (t->right != NULL && t->right->val == 0) return true;
    return false;
}
