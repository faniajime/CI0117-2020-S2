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
        int id;        
        Estrategies my_strategy;
        void randomStrategy();
    public:
        Mario mario(this->id);
        Player(int);
        ~PLayer();
        void setEstrategy(Estrategies);
        int getId();

    
};
#endif

