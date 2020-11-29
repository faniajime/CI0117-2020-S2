#include "world.h" 
#include <iostream>
#include <array>

using namespace std;

World::World(World world, vector<vector<int>> baseWorld){
    world.myWorld = baseWorld;
    world.world_size = baseWorld.size();
};

World::~World(){
    free(world->myWorld);
};

vector<int> World::getNext(World world){
    world.current_position+=1;
    return world.myWorld[world.current_position];
};

void World::refreshWorld(World world, int koopatroopas){
    if(koopatroopas){
        world.myWorld = baseWorld;
        world.addElement(koopatroopas);
    }else{
        world.myWorld = baseWorld;
    }
};
void World::addElement(World world, int koopatroopas){
    if( world.world_size - world.current_position < 10){
        pass;
    }else{
        for( int i =0; i<koopatroopas; i++){
            world.myWorld[world.current_position+10].push_back(2);
        }
    }
};
