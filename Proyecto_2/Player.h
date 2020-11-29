#ifndef PLAYER_H
#define PLAYER_H

#include "Mario.h"
using namespace std;

enum Estrategies
{
    R ,// random
    L,// less coins
    M,// more coins
    A // 
};

class PLayer
{
    private:
        Mario mario;
        //World world;************es necesario?*********
    public:
        Player();
        ~PLayer();
        void chooseEstrategy(Estrategies estrategy);


    
};
#endif

