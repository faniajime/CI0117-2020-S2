#include "world.h" 
#include <iostream>
#include <array>

using namespace std;

World::World( ){
    this->myWorld = baseWorld;
    this->world_size = baseWorld.size();
    this->current_position = 0;
};

World::~World()
{
    
};

vector<int> World::getNext(){
    return this->myWorld[this->current_position];
};


void World::refreshWorld( ){
    this->myWorld.assign(baseWorld.begin(), baseWorld.end());
    this->current_position=0;
};

void World::addElement( int koopatroopas){
    if( this->world_size - this->current_position > 10){
        for( int i =0; i<koopatroopas; i++){
            this->myWorld[this->current_position+10].push_back(2);
        }
    }
};

int World::getIndexPosition()
{
    return this->current_position;
}
