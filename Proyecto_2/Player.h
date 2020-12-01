#ifndef PLAYER_H
#define PLAYER_H

#include "Mario.h"
using namespace std;

enum Estrategies
{
    R = 1,// random
    L = 2,// less coins
    M = 3,// more coins
    A = 4 // attacker
};

class Player
{
    private:
        Mario mario;
        Estrategies my_strategy;
        //World world;************es necesario?*********
    public:
        Player();
        PLayer(Estrategies)
        ~PLayer();
        void setEstrategy();
    
};
#endif

