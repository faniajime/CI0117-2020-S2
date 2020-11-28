 #include <iostream>
 #include <string>

 using namespace std;


enum Elements
{
    Hole =1,
    Goomba =2,
    KoopaTroopa =3,
    Coin = 4,
};


class World_Element
{
    public:
        string* get_actions(){};
    private:
        string* actions;
       
};

class Coin: public World_Element
{
    const string actions[2] = {"NO_JUMP", "JUMP"};
    void get_actions(){};
    double get_action_probability(string action){};

};

class Hole: public World_Element
{
    const string actions[2] = {"NO_JUMP", "JUMP"};
    void get_actions(){};
    double get_action_probability(string action){};

};

class littleGoomba: public World_Element
{
    const string actions[3] = {"NO_JUMP", "JUMP_PASS", "JUMP_ATTACK"};
    void get_actions(){};
    double get_action_probability(string action){};
    void kill_enemy(){};

};

class koopaTroopa: public World_Element
{
    const string actions[3] = {"NO_JUMP", "JUMP_PASS", "JUMP_ATTACK"};
    void get_actions(){};
    double get_action_probability(string action){};
    void kill_enemy(){};
    
};

 class World
 {    
    void initialize(){};
    void destroy_world(){};
    public:
        World_Element* my_world;
        void getNext();
    private:
        int current_position;
        void updateWorld(){};
 };