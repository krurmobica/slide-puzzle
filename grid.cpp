#include "grid.h"
#include "tile.h"

#include <iostream>
using namespace std;

Grid::Grid(int s, QRect area)
{    
    newGrid(s, area);
}

void Grid:: newGrid(int s, QRect area){

    size = s;
    maxSize = size*size;
    tileArray = new Tile[maxSize]; // initialize Tile Array

    //calculate grid sizes with rects etc
    int tileSize;
    int x_coord = area.x();
    int y_coord = area.y();
    int h = area.height();
    int w = area.width();

    // calculate tile size based on available area
    tileSize = (int)floor(h/size);

    if (w < h) tileSize = (int)floor(w/size);

    int n = 0;
    for( int y=0; y < size; y++ )    {

        for( int x=0; x < size; x++ )
        {
            Tile t(n,QPoint(x,y),QRect(x_coord,y_coord,tileSize,tileSize));
            this->setTile(x, y, t);
            n++;
            x_coord+=tileSize;
        }
        x_coord = area.topLeft().x(); //reset x coord
        y_coord+=tileSize;
    }
    shuffle(); // not implemented
    this->print();
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

void Grid::print()
{
    cout << "size:" << this->size << endl;
    for( int i=0; i < size; i++ )
    {
        for( int j=0; j < size; j++ )
        {
            cout << this->getTile(j,i)->getValue();
            int *nbr = this->getTile(j,i)->getNeighborValues();
            for (int c = 0; c < 4 ; c++){
               cout << nbr[c] << " ";
            }
            cout << endl;
        }
    }
}
