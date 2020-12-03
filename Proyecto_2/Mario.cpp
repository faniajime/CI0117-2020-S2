#include "Mario.h"
#include "world.h"

using namespace std;


Mario::Mario(int id)
{
    this->coins = 0;
    this->isActive = 1;
    this->id = id;
    this->world = new World();
}

Mario:: ~Mario()
{

}

void Mario::addCoins()
{
    this->coins++;
}

int Mario::die(){
    this->isActive == 0;
    return 1;
}

int Mario::getCoins()
{
    return this->coins;
}

int Mario:: getLocation()
{
    return this->world->getCurrentPosition();
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
    int action = 0;
    double random = (double) rand()/RAND_MAX;
    if(element== Goomba || element == KoopaTroopa)
    {
       action = foundEnemy(element,random);
    }
    else if(element== Hole)
    {
        foundAHole(random);
    }
    else
    {
        if(foundACoin(random==1))
        {
            action = 1;
        }
    }
    return action;
 }

 int Mario::foundACoin(double probability)
 {
    int catchedCoin = 0;
    if(probability< 0.5)
    {
        dontJump();
    }
    else
    {
        jump();
        catchedCoin = 1;
    }    
    return catchedCoin;
 }

 int Mario :: foundEnemy(Elements element,double probability)
 {
    int action = 0;
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
            action = 2;
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
            action = 2;
        }        
    }    
    return action;
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