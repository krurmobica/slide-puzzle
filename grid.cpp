#include "grid.h"
#include "tile.h"

Grid::Grid(int s, QRect area)
{    
    newGrid(s, area);
}

void Grid:: newGrid(int s, QRect area){

    size = s;
    maxSize = size*size;
    tileArray = new Tile[maxSize]; // initialize Tile Array

    //calculate grid sizes with rects etc

    int x_coord = area.x();
    int y_coord = area.y();
    int h = area.height();
    int w = area.width();
    areaWidth = w;
    areaHeight = h;

    // calculate tile size based on available area
    tileSize = (int)floor(h/size);

    if (w < h) tileSize = (int)floor(w/size);
    x_offset = (w - tileSize*size) / 2;
    x_coord += x_offset;
    int n = 1;
    for( int y=0; y < size; y++ )    {

        for( int x=0; x < size; x++ )
        {
            if (n == maxSize){
                n = 0;
            }
            Tile t(n,QPoint(x,y),QRect(x_coord,y_coord,tileSize,tileSize));
            this->setTile(x, y, t);
            bindTile(x,y);
            if (n == 0) {
                this->blankTile = this->getTile(x, y);
            }
            n++;
            x_coord+=tileSize;
        }
        x_coord = area.topLeft().x() + x_offset; //reset x coord and center
        y_coord+=tileSize;
    }
    shuffle();
}

void Grid::shuffle(){
    node* lastNode = new node;
    lastNode->val = 0;
    bool corner = false;
    int i = 0;
    do{
        Tile *next = blankTile->randomNeighbor();
        int tileValue = next->getValue();
        if (tileValue != lastNode->val){
            if (i > size*size && next->getPosition().x() == 0 && next->getPosition().y() == 0) corner = true;
            node *n = new node;
            n->val = tileValue;
            n->tile = blankTile;
            n->last = lastNode;
            lastNode = n;
            i++;

        } else { //ignore back forth moves
            if (lastNode->last != NULL){
                lastNode = lastNode->last;
            } else {
                lastNode = new node;
                lastNode->val = 0;
            }
        }

        next->move();
        blankTile = next;
    }while(corner == false);

    this->lastNode = lastNode;
}

void Grid::setTile(int x, int y, Tile t){
    if (x < size && y < size){
        int index = x+y*size; //convert 2d coordinates to 1d
        tileArray[index] = t;
    }
}

Tile* Grid:: getTile(int x, int y){
    if (x < size && y < size){
        int index = x+y*size; //convert 2d coordinates to 1d
        return &tileArray[index];
    }
    return blankTile;
}

void Grid::bindTile(int x, int y){
    //bind neighboring tiles together
    Tile *thisTile = this->getTile(x,y);
    if (x!=0) // x = 0 has no Left neighbor
    {
        Tile *leftTile = this->getTile(x-1,y);
        thisTile->neighbor[Tile::Left] = leftTile;
        leftTile->neighbor[Tile::Right] = thisTile;
    }
    if (y>0) //y=0 has no Up neighbor
    {
        Tile *upTile = this->getTile(x,y-1);
        thisTile->neighbor[Tile::Up] = upTile;
        upTile->neighbor[Tile::Down] = thisTile;
    }
}

Tile* Grid::getTileArray(){
    return tileArray;
}
int Grid::getSize(){
    return size;
}
int Grid::getMaxSize(){
    return maxSize;
}
int Grid::getTileSize(){
    return tileSize;
}
Tile* Grid::blank(){
    return blankTile;
}
Tile* Grid::getTile(QPoint pos){
    int x = pos.x();
    int y = pos.y();
    if (x> x_offset) x -= x_offset;
    //cast x,y mouse coordinates to tileArray index
    int index = (int)(floor(x/tileSize) + size*floor(y/tileSize));
    return &tileArray[index];
}
int Grid::getLast(){
    return lastNode->val;
}
bool Grid::isFinished(){
    //check if game is finished
    if (tileArray[maxSize-1].getValue() == 0){ // blank tile is in correct place
        for (int i = 0; i<maxSize-1; i++){
            if (tileArray[i].getValue() != i+1) return false;
        }
        return true;
    }
    return false;
}

int Grid::getWidth(){
   return size*tileSize;
}

void Grid::move(Tile* t){

    node *n = new node;
    n->val = t->getValue();
    n->tile = blankTile;
    n->last = lastNode;
    lastNode = n;
    t->move();
    blankTile = t;
}
void Grid::undo(){
    if (size > 0) {
        if (lastNode->val){
            lastNode->tile->move();
            blankTile = lastNode->tile;
            lastNode = lastNode->last;
        }
    }
}

void Grid::resize(QRect area){
    if (area.width() != areaWidth || area.height() != areaHeight){
        int x_coord = area.x();
        int y_coord = area.y();
        int h = area.height();
        int w = area.width();

        // calculate tile size based on available area
        int newTileSize = (int)floor(h/size);

        if (w < h) newTileSize = (int)floor(w/size);

        if(tileSize != newTileSize){
            tileSize = newTileSize;
            x_offset = (w - tileSize*size) / 2; //center on x axis
            x_coord += x_offset;
            for( int y=0; y < size; y++ ) {
                for( int x=0; x < size; x++ )
                {
                    Tile* t = &tileArray[x+size*y];
                    t->setRectangle(QRect(x_coord,y_coord,tileSize,tileSize));
                    x_coord+=tileSize;
                }
                x_coord = area.topLeft().x()+x_offset; //reset x coord
                y_coord+=tileSize;
            }
        }
    }
}
int Grid::getXOffset(){
    return x_offset;
}
