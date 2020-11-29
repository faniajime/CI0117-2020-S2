 #include <iostream>
 #include <string>

 using namespace std;

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
    void get_action(){};

};

class Hole: public World_Element
{
    const string actions[2] = {"NO_JUMP", "JUMP"};
    void get_actions(){};

};

class littleGoomba: public World_Element
{
    const string actions[3] = {"NO_JUMP", "JUMP_PASS", "JUMP_ATTACK"};
    void get_actions(){};

};

class koopaTroopa: public World_Element
{
    const string actions[3] = {"NO_JUMP", "JUMP_PASS", "JUMP_ATTACK"};
    void get_actions(){};
    
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
