#include "Player.h"


Player::Player()
{
    this->mario = new Mario(my_id);
    setStrategy();
}

Player::PLayer(Estrategies strategy)
{
    this->mario = new Mario(my_id);
    this->strategy = strategy;

}

Player::~PLayer()
{

}

void PLayer::setStrategy()
{
    int random = rand()%4;
    if(random ==1)
    {
        this->strategy = R;
    }
    else if(random ==2)
    {
        this->strategy = L;
    }
    else if(random==3)
    {
        this->strategy = M;
    }
    else
    {
        this->strategy = A;
    }   
     
}
