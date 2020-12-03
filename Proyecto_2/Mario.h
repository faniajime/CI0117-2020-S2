#ifndef MARIO_H
#define MARIO_H

#include <math.h>
#include <time.h>
#include "world.h"

using namespace std;


class Mario
{
    private:
        int location;
		int coins;
        int isActive;
        int id;  

    public:
        World* world;
        Mario(int);
        ~Mario();
        int getCoins();
        void addCoins();
        int getLocation();
        void setLocation();//********************revisar********************************
        int getId();
        int isAlive();
        int chooseAction(Elements); //Mario choose what to do when found an element
        int foundACoin(double);
        int foundEnemy(Elements,double);
        void foundAHole(double);
        void jump(); // Mario jump if there's a hole, an enemy or a coin
        void jumpAndDefeat (); // Mario jump and kill a goomba or koopa troopa
        void dontJump();// Mario doesn't jump and get killed
};
#endif

