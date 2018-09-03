#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QPoint>
#include <QRect>


class Tile
{
private:
    int val;
    QPoint pos;
    QRect rectangle;

public:
    enum Neighbor {
        // neighboring tiles
        Right,
        Left,
        Up,
        Down,
    };

    Tile();
    Tile(int v, QPoint p, QRect r): val(v), pos(p), rectangle(r){}

    void move();
    bool isBlank();
    void setValue(int v);
    void setPosition(QPoint p);
    void setRectangle(QRect r);
    int getValue();
    int * getNeighborValues();
    QPoint getPosition();
    QRect rect();
    Tile* neighbor[4]; //directions
};

#endif // TILE_H
