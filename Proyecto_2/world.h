#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <vector>
#include "world.h"

using namespace std;


const vector<vector<int>> baseWorld = {{},{},{},{},{},{},{},{},{4},{},{4},{2,4},{},{},{},{},{},{},{},{2},{},{},{},{},{2},{2},{},{},{},{},{},{4},{},{1},{1},{},{},{},{4},{},{},{},{1,1},{1},{},{},{4},{2},{2},{4},{},{},{4,3},{4},{},{4},{2},{2},{},{},{},{2},{2},{2},{2},{},{},{},{},{},{},{},{},{},{},{1},{1},{},{},{},{},{},{},{},{4},{},{2},{2},{},{},{},{},{},{},{},{},{},{},{5}};

enum Elements
{
    Hole = 1,
    Goomba = 2,
    KoopaTroopa = 3,
    Coin = 4,
    Flag = 5
};


class World
{  

    public:
        World();
        ~World();
        vector<int> getNext();
        vector<int> getPosition();
        int getCurrentPosition();
    private:
        vector<vector<int>> myWorld;
        int world_size;
        int current_position;
        void refreshWorld();
        void addElement(int);
};
#endif
