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

class Mario;

class Player
{
    friend class Mario;
    private:
        int id;        
        Estrategies strategy;
        void randomEstrategy();
    public:
        Mario* mario;
        Player(int);
        ~Player();
        void setEstrategy(Estrategies);
        int getId();

    
};
#endif

