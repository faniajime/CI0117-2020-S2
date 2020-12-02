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
        Estrategies strategy;
        void randomStrategy();
    public:
        Mario mario(int);
        Player(int);
        ~Player();
        void setEstrategy(Estrategies);
        int getId();

    
};
#endif

