#include "world.h" 
#include <iostream>
#include <array>

using namespace std;

World::World( vector<vector<int>> baseWorld){
    this->myWorld = baseWorld;
    this->world_size = baseWorld.size();
};

World::~World(){
    free(this->myWorld);
};

vector<int> World::getNext(){
    this->current_position+=1;
    return this->myWorld[this->current_position];
};

void World::refreshWorld( int koopatroopas){
    if(koopatroopas){
        this->myWorld = baseWorld;
        this->addElement(koopatroopas);
    }else{
        this->myWorld = baseWorld;
    }
};
void World::addElement( int koopatroopas){
    if( this->world_size - this->current_position > 10){
        for( int i =0; i<koopatroopas; i++){
            this->myWorld[this->current_position+10].push_back(2);
        }
    }
};
