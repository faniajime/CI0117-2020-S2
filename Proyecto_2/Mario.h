#ifndef MARIO_H
#define MARIO_H

#include <math.h>
#include <time.h>
#include "world.h"
#include "Mario.h"

using namespace std;


class Mario
{
    private:
        int location;
		int coins;
        int isActive;
        int id;  

    public:
        int printer;
        World* world;
        Mario(int);
        ~Mario();
        int getCoins();
        void addCoins();
        int getLocation();
        int getId();
        int isAlive();
        int die();
        int chooseAction(Elements); //Mario choose what to do when found an element
        int foundACoin(Elements, double);
        int foundEnemy(Elements,double);
        void foundAHole(Elements, double);
        void jump(Elements); // Mario jump if there's a hole, an enemy or a coin
        void jumpAndDefeat (Elements); // Mario jump and kill a goomba or koopa troopa
        void dontJump(Elements);// Mario doesn't jump and get killed
};
#endif

