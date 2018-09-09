#ifndef GRID_H
#define GRID_H
#include <QObject>
#include <QRect>
#include "tile.h"

struct node;

class Grid : public QObject
{
    Q_OBJECT
private:

    int size=0; // number of tiles on each side
    int maxSize;
    int tileSize; // pixel size of a Tile
    Tile* blankTile;
    Tile* tileArray;
    node* lastNode;
    int areaWidth;
    int areaHeight;
    int x_offset;

public:
    Grid(){}
    Grid(int s, QRect area);
    void newGrid(int s, QRect area);
    void shuffle();
    void resize(QRect area); //recalculate and redraw on window resize
    void bindTile(int x, int y);
    void setTile(int x, int y, Tile);
    Tile* getTile(int x, int y);
    Tile* getTileArray();
    int getSize();
    int getMaxSize();
    int getTileSize();
    Tile* blank();
    Tile* getTile(QPoint pos);
    int getLast();
    void move(Tile* t);
    void undo();
    bool isFinished();
    int getWidth();
    int getXOffset();
};

struct node{
    int val;
    Tile *tile;
    node *last;
};

#endif // GRID_H
