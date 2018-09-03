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

QPoint Tile::getPosition(){
    return pos;
}

QRect Tile::rect(){
    return rectangle;
}
