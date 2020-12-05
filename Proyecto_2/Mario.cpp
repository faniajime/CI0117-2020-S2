#include "Mario.h"
#include "world.h"

using namespace std;


Mario::Mario(int id)
{
    this->coins = 0;
    this->isActive = 1;
    this->id = id;
    this->world = new World();
    this->printer = 0;
}

Mario:: ~Mario()
{

}

void Mario::addCoins()
{
    this->coins++;
}

int Mario::die(){
    this->isActive = 0;
    return isActive;
}

int Mario::getCoins()
{
    return this->coins;
}

int Mario:: getLocation()
{
    return this->world->getIndexPosition();
}


int Mario:: getId()
{
    return this->id;
}

int Mario::isAlive()
{
    int alive = 0;
    if(this->isActive)
        alive = 1;
    return alive;
}

int Mario::chooseAction(Elements element) //Mario choose what to do when found an object
{
    srand(time(0));
    int action = 0;
    double random = (double) rand()/RAND_MAX; //se genera una probabilidad al azar
    if(element== Goomba || element == KoopaTroopa)
    {
       action = foundEnemy(random,element);
    }
    else if(element== Hole)
    {
        foundAHole(random,element);
    }
    else
    {
        if(foundACoin(random, element))
        {
            action = 1;
        }
    }
    return action;
}

int Mario::foundACoin(double probability, Elements element)
{
    int catchedCoin = 0;
    if(probability< 0.5)
    {
        dontJump(element);
    }
    else
    {
        jump(element);
        catchedCoin = 1;
    }    
    return catchedCoin;
}

int Mario :: foundEnemy(double probability,Elements element)
{
    int action = 0;
    if(element== Goomba)
    {
        if(probability<= 0.05)
        {
            dontJump( element);
            this->isActive = 0;
        }
        else if(probability > 0.05 && probability<= 0.6)
        {
            jump(element);
        }
        else
        {
            jumpAndDefeat(element);
            action = 2;
        }        
    }
    else
    {
        if(probability <= 0.10)
        {
            dontJump(element);            
        }
        else if(probability> 0.10 && probability <= 0.63)
        {
            jump( element);
        }
        else
        {
            jumpAndDefeat( element);
            action = 2;
        }        
    }    
    return action;
}
     
void Mario::foundAHole(double probability, Elements element)
{
    if(probability<=0.5)
    {
        dontJump(element);
    }
    else{
        jump(element);
    }
        
}

 void Mario::jump(Elements element) // Mario jump if there's a hole, goomba or koopa troopa
{
    if (element == Coin){// si el elemento es una moneda
        if(this->id == this->printer ){
            cout << " Mario ha brincado y ha atrapado la moneda. ";
        }
    }else if(element == Goomba){
        if(this->id == this->printer ){
            cout << " Mario ha brincado, ha logrado evadir a un Goomba. ";
        }
    }else if(element == KoopaTroopa){
        if(this->id == this->printer ){
            cout << " Mario ha brincado, ha logrado evadir a un Koopa Troopa. ";
        }
    }else{
        if(this->id == this->printer ){
            cout << " Mario ha brincado y ha evitado caer en un hueco. ";
        }
    }
}

void Mario::jumpAndDefeat (Elements element) // Mario jump and kill a goomba or koopa troopa
{
    if(element == Goomba){
        if(this->id == this->printer ){
            cout << " Mario ha brincado y matado a un Goobma. ";
        }
    }else{
        if(this->id == this->printer ){
            cout << " Mario ha brincado y matado a un Koopa Troopa. ";
        }
    }
}

void Mario:: dontJump(Elements element )// si mario no brinca
{
    if (element == Coin){// si el elemento es una moneda
        if(this->id == this->printer ){
            cout << " Mario no ha brincado y no ha atrapado la moneda. ";
        }
    }else if(element == Goomba){
        if(this->id == this->printer ){
            cout << " Mario no ha brincado, ha sido matado por un goomba. ";
        }
        this->isActive = 0;
    }else if(element == KoopaTroopa){
        if(this->id == this->printer ){
            cout << " Mario no ha brincado, ha sido matado por un Koopa Troopa. ";
        }
        this->isActive = 0;
    }else{
        if(this->id == this->printer ){
            cout << " Mario no ha brincado, ha caido en un hueco. ";
        }
        this->isActive = 0;
    }
}