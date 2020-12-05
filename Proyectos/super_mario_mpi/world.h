#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <vector>
#include "world.h"

using namespace std;


const vector<vector<int>> baseWorld = {{},{},{},{},{},{},{},{},{4},{},{4},{2,4},{},{},{},{},{},{},{},{2},{},{},{},{},{2},{2},{},{},{},{},{},{4},{},{1},{1},{},{},{},{4},{},{},{},{1,1},{1},{},{},{4},{2},{2},{4},{},{},{4,3},{4},{},{4},{2},{2},{},{},{},{2},{2},{2},{2},{},{},{},{},{},{},{},{},{},{},{1},{1},{},{},{},{},{},{},{},{4},{},{2},{2},{},{},{},{},{},{},{},{},{},{},{}};

enum Elements
{
    Hole = 1,
    Goomba = 2,
    KoopaTroopa = 3,
    Coin = 4,
};


class World
{  

    public:
        World();
        ~World();
        vector<int> getNext();
        vector<int> getPosition();
        int getIndexPosition();
        int world_size;
        void refreshWorld();
        void addElement(int, int);
        int current_position;

    private:
        vector<vector<int>> myWorld;
        

};
#endif
