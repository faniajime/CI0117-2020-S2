#ifndef MARIO_H
#define MARIO_H
using namespace std;


class Mario
{
    private:
        int location;
		int coins;
        int isActive;
		
        

    public:
        Mario();
        ~Mario();
        int getCoins();
        void setCoins(int);
        int isAlive();
        void chooseAction(Elements); //Mario choose what to do when found an element
        void foundACoin(double);
        int foundEnemy(Elements,double);
        void foundAHole(double);
        void jump(); // Mario jump if there's a hole, an enemy or a coin
        void jumpAndDefeat (); // Mario jump and kill a goomba or koopa troopa
        void dontJump();// Mario doesn't jump and get killed
};
#endif

