#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QColor>
#include <cstdlib>

#include <iostream>
using namespace std;

//using std::vector;
//using std::any_of;


class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    int mGridSize = 3;
    bool initialized = false;
    bool cheats = false;
    bool finished = false;
    bool autosolve = false;

//    void randomizeGrid()
//    {
//        int predefined[] = {1, 2, 3, 2, 5, 7, 8, 6};

//        int size = mGridSize;
//        int maxValue = size*size;
//        this->mMoves = 0;
//        this->mGridArraySize = maxValue;
//        this->gridElements = new gridElement[maxValue]; //use 1d array with size of size^2
//        //calculate grid sizes with rects etc
//        int tileSize;
//        int x = this->rect().x();
//        int y = this->rect().y();
//        int h = this->rect().height();
//        int w = this->rect().width();
//        int d = 1;

//        tileSize = (int)floor(h/size);

//        if (w < h) tileSize = (int)floor(w/size);

//        this->mTileSize = tileSize;

//        vector<int> used;
//        int counter = 1;
//        //for (int i=0; i<maxValue; i ++){
//        for( int i=0; i < size; i++ )
//        {

//            for( int j=0; j < size; j++ )
//            {
//                int n;
//                //n = predefined[counter-1];
//                if (counter < maxValue){
//                    do {
//                         n = (rand()%(maxValue-1))+1; // exclude 0
//                    }
//                    while(any_of(used.begin(), used.end(), compare(n)));
//                } else {
//                    n = 0;
//                }
//                used.push_back(n);

//                counter++;

//                QRect r = QRect(x,y,tileSize,tileSize);
//                tile t = {n, r, NULL, NULL, NULL, NULL};

//                gridElement ge = {r, t};
//                int index = j+i*size;
//                this->gridElements[index] = ge; //offset as replacement to 2d array
//                bindTile(index);
//                if (n == 0) {
//                    this->blankTile = &this->gridElements[index].tile;
//                    this->lastTile = this->blankTile;
//                }
//                x+=tileSize;
//            }
//            x = this->rect().topLeft().x(); //reset x coord
//            y+=tileSize;
//        }
//        initialized = true;
//        finished = false;
//    }

    void solvableGrid()
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

    void randomize(){
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
//        node *n = lastNode;
//        do{
//            if (n->last->val != 0){
//                n = n->last;
//            } else {
//                break;
//            }
//        }while(true);

        this->lastNode = lastNode;
    }



    bool isFinished(){
        //check if game is finished
        if (gridElements[mGridArraySize-1].tile.val == 0){ // blank tile is in correct place
            for (int i = 0; i<mGridArraySize-1; i++){
                if (gridElements[i].tile.val != i+1) return false;
            }
            return true;
        }
        return false;
    }

    void undoMove(){
        //remember all moves
        swap(lastNode->tile, -1);
        if (lastNode->last->val != 0){
            lastNode = lastNode->last;
        } else {
            lastNode = new node;
        }
        this->repaint();
    }

    void updateMoves(int moves, int last); //send signal to update moves

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent ( QMouseEvent * event ) override;

    struct tile{
        int val;
        QRect rec;

        tile *up;
        tile *down;
        tile *left;
        tile *right;
    };

    struct node{
        int val;
        tile *tile;
        node *last;
    };

    struct gridElement{
        QRect rec;
        tile tile;
    };

    void bindTile(int index){
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

    tile *getDirection(int d, tile *t){
        switch(d){
            case 0: return t->up;
            case 1: return t->down;
            case 2: return t->left;
            case 3: return t->right;
        }
        return 0;
    }

    tile *getTile(QPoint point){
        int x = point.x();
        int y = point.y();
        int index = getIndex(x,y);
        if (index < this->mGridArraySize){ //out of bounds
            return &this->gridElements[index].tile;
        }
        return this->blankTile; //return blank tile as a false
    }

    void swap(tile *t, int move)
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

    void swapHidden(tile *t)
    {
        this->blankTile->val = t->val;
        t->val = 0;
        this->blankTile = t;
    }

    bool isTileMovable(tile *t){
        if (t->up != NULL && t->up->val == 0) return true;
        if (t->down != NULL && t->down->val == 0) return true;
        if (t->left != NULL && t->left->val == 0) return true;
        if (t->right != NULL && t->right->val == 0) return true;
        return false;
    }

    int getIndex(int x, int y)
    {
        return (int)(floor(x/this->mTileSize) + mGridSize*floor(y/this->mTileSize));
    }

    int getIndex(tile *t)
    {
        int x = t->rec.x();
        int y = t->rec.y();
        return (int)(floor(x/this->mTileSize) + mGridSize*floor(y/this->mTileSize));
    }

    struct compare
    {
        int k;
        compare(int const &i): k(i) {}
        bool operator()(int const &i)
        {
            return(i == k);
        }
    };

    void drawTileGrid();
    void move(QPoint point);

signals:
  void  sendMoves( int moves, int last);

public slots:

private:
    QColor mBackgroundColor;
    QColor mBorderColor;
    QColor mPenColor;


    int mTileSize;
    int mGridArraySize;
    int mMoves = 0;
    QPainter p();
    tile *blankTile;
    tile *lastTile;
    node *lastNode;
    gridElement  *gridElements;

};

#endif // RENDERAREA_H
