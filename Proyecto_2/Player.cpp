#include "Player.h"


Player::Player(int id)
{   
    this->id = id;
    this->mario = new Mario(id);
    randomEstrategy();
}

Player::~Player()
{

}

void Player::randomEstrategy()
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

void Player::setEstrategy(Estrategies strategy)
{
    this->strategy = strategy;
}
