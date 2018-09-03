#ifndef GRID_H
#define GRID_H
#include <QObject>
#include <QRect>
#include "tile.h"

class Grid : public QObject
{
    Q_OBJECT
private:

    int size; // number of tiles on each side
    int maxSize;
    int tileSize; // pixel size of a Tile
    Tile* blankTile;
    Tile* tileArray;

public:

    Grid(int s, QRect area);
    void newGrid(int s, QRect area);
    void shuffle();
    void resize(); //recalculate and redraw on window resize
    void draw();
    void print();
    void bindTile(int x, int y);
    void setTile(int x, int y, Tile);
    Tile* getTile(int x, int y);

};

#endif // GRID_H
