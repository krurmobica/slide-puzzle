#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QColor>
#include <cstdlib>

#include <iostream>
using namespace std;

struct tile;
struct node;
struct gridElement;

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

    void solvableGrid();

    void randomize();

    bool isFinished();

    void undoMove();

    void updateMoves(int moves, int last); //send signal to update moves

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent ( QMouseEvent * event ) override;

    void bindTile(int index);

    tile *getDirection(int d, tile *t);

    tile *getTile(QPoint point);

    void swap(tile *t, int move);

    void swapHidden(tile *t);

    bool isTileMovable(tile *t);

    int getIndex(int x, int y);

    int getIndex(tile *t);

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
    tile *blankTile;
    tile *lastTile;
    node *lastNode;
    gridElement  *gridElements;

};

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

#endif // RENDERAREA_H
