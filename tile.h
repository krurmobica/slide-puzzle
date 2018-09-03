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
    Tile(int v, QPoint p, QRect r): val(v), pos(p), rectangle(r){
        neighbor[Tile::Up] = nullptr;
        neighbor[Tile::Down] = nullptr;
        neighbor[Tile::Left] = nullptr;
        neighbor[Tile::Right] = nullptr;
    }

    void move();
    bool isBlank();
    bool isMovable();
    void setValue(int v);
    void setPosition(QPoint p);
    void setRectangle(QRect r);
    int getValue();
    int* getNeighborValues();
    QPoint getPosition();
    QRect rect();
    Tile* neighbor[4];
};

#endif // TILE_H
