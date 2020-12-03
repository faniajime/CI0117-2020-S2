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
    this->current_position+=1;
    return this->myWorld[this->current_position];
};

vector<int> World::getPosition(){
    return this->myWorld[this->current_position];
};

void World::refreshWorld( ){
    this->myWorld = baseWorld;
};

void World::addElement( int koopatroopas){
    if( this->world_size - this->current_position > 10){
        for( int i =0; i<koopatroopas; i++){
            this->myWorld[this->current_position+10].push_back(2);
        }
    }
};

int World::getCurrentPosition()
{
    return this->current_position;
}
