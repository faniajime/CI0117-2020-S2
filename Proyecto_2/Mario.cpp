#include <math.h>
#include <time.h>
#include "Mario.h"
#include "world.h"

using namespace std;


Mario::Mario()
{
    this->coins = 0;
    this->isActive = 1;
    this->location = 0;
}

Mario:: ~Mario()
{

}

void Mario::setCoins(int coins)
{
    this->coins+= coins;
}

int Mario::getCoins()
{
    return this->coins;
}


int Mario::isAlive()
{
    int alive = 0;
    if(this->isActive ==1)
        alive = 1;
    return alive;
}

 void Mario::chooseAction(Elements element) //Mario choose what to do when found an object
 {
    srand(time(0));
    double random = (double) rand()/RAND_MAX;
    if(element== Goomba || element == KoopaTroopa)
    {
       foundEnemy(element,random);
    }
    else if(element== Hole)
    {
        foundAHole(random);
    }
    else
    {
        foundACoin(random);
    }
 }

 void Mario::foundACoin(double probability)
 {
    if(probability< 0.5)
    {
        dontJump();
    }
    else
    {
        jump();
    }    
 }

 int Mario :: foundEnemy(Elements element,double probability)
 {
    if(element== Goomba)
    {
        if(probability<= 0.05)
        {
            dontJump();
            this->isActive = 0;
        }
        else if(probability > 0.05 && probability<= 0.6)
        {
            jump();
        }
        else
        {
            jumpAndDefeat();
        }        
    }
    else
    {
        if(probability <= 0.10)
        {
            dontJump();
            this->isActive = 0;
        }
        else if(probability> 0.10 && probability <= 0.63)
        {
            jump();
        }
        else
        {
            jumpAndDefeat();
        }        
    }    
 }
     
 int foundHole(double probability)
 {

 }

 void Mario::jump() // Mario jump if there's a hole, goomba or koopa troopa
 {

 }

 void Mario::jumpAndDefeat () // Mario jump and kill a goomba or koopa troopa
 {

 }

 void Mario:: dontJump()// Mario doesn't jump and get killed
 {
     
 }