#include "grid.h"
#include "tile.h"

#include <iostream>
using namespace std;

Grid::Grid(int s, QRect area)
{    

}

void Grid::setTile(int x, int y, Tile t){
    if (x < size && y < size){
        int index = x+y; //convert 2d coordinates to 1d
        tileArray[index] = t;
    }
}

Tile* Grid:: getTile(int x, int y){
    if (x < size && y < size){
        int index = x+y; //convert 2d coordinates to 1d
        return &tileArray[index];
    }
}
