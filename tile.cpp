#include "tile.h"

Tile::Tile()
{
    neighbor[Tile::Up] = nullptr;
    neighbor[Tile::Down] = nullptr;
    neighbor[Tile::Left] = nullptr;
    neighbor[Tile::Right] = nullptr;
}

void Tile::move(){
    if (!this->isBlank()){
        //check if blank is a neighbor

    }
}

bool Tile::isBlank(){
    if (val == 0) return true;
    return false;
}

bool Tile::isMovable(){
    int* n_values = this->getNeighborValues();
    for (int i=0; i<4; i++){
        if(n_values[i]==0) return true;
    }
    return false;
}

void Tile::setValue(int v){
    val = v;
}

void Tile::setPosition(QPoint p){
    pos = p;
}

void Tile::setRectangle(QRect r){
    rectangle = r;
}

int Tile::getValue(){
    return val;
}

int* Tile::getNeighborValues(){
    int *r = new int[4];
    for (int i=0; i<4;i++) r[i]=-1;
    if (neighbor[Tile::Up] != nullptr)
        r[0]=neighbor[Tile::Up]->val;
    if (neighbor[Tile::Down] != nullptr)
        r[1]=neighbor[Tile::Down]->val;
    if (neighbor[Tile::Left] != nullptr)
        r[2]=neighbor[Tile::Left]->val;
    if (neighbor[Tile::Right] != nullptr)
        r[3]=neighbor[Tile::Right]->val;
    return r;
}

QPoint Tile::getPosition(){
    return pos;
}

QRect Tile::rect(){
    return rectangle;
}
