#include "Mario.h"
#include "world.h"

using namespace std;


Mario::Mario(int id)
{
    this->coins = 0;
    this->isActive = 1;
    this->location = 0;
    this->id = id;
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

int Mario:: getLocation()
{
    return this->location;
}

void Mario:: setLocation()
{
    this->location+=1;
}

int Mario:: getId()
{
    return this->id;
}

int Mario::isAlive()
{
    int alive = 0;
    if(this->isActive ==1)
        alive = 1;
    return alive;
}

 int Mario::chooseAction(Elements element) //Mario choose what to do when found an object
 {
    srand(time(0));
    int killedEnemy = 0;
    double random = (double) rand()/RAND_MAX;
    if(element== Goomba || element == KoopaTroopa)
    {
       killedEnemy = foundEnemy(element,random);
    }
    else if(element== Hole)
    {
        foundAHole(random);
    }
    else
    {
        foundACoin(random);
    }
    return killedEnemy;
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
    int killedEnemy = 0;
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
            killedEnemy = 1;
        }        
    }
    else
    {
        if(probability <= 0.10)
        {
            dontJump();            
        }
        else if(probability> 0.10 && probability <= 0.63)
        {
            jump();
        }
        else
        {
            jumpAndDefeat();
            killedEnemy = 1;
        }        
    }    
    return killedEnemy;
 }
     
 void Mario::foundAHole(double probability)
 {
    if(probability<=0.5)
    {
        dontJump();
    }
    else 
        jump();
 }

 void Mario::jump() // Mario jump if there's a hole, goomba or koopa troopa
 {

 }

 void Mario::jumpAndDefeat () // Mario jump and kill a goomba or koopa troopa
 {

 }

 void Mario:: dontJump()// Mario doesn't jump and get killed
 {
    this->isActive = 0;
 }