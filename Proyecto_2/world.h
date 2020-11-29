#include <iostream>
#include <string>
#include <vector>

using namespace std;


const vector<vector<int>> baseWorld = {{}{}{}{}{}{}{}{}{4}{}{4}{2,4}{}{}{}{}{}{}{}{2}{}{}{}{}{2}{2}{}{}{}{}{}{4}{}{1}{1}{}{}{}{4}{}{}{}{1,1}{1}{}{}{4}{2}{2}{4}{}{}{4,3}{4}{}{4}{2}{2}{}{}{}{2}{2}{2}{2}{}{}{}{}{}{}{}{}{}{}{1}{1}{}{}{}{}{}{}{}{4}{}{2}{2}{}{}{}{}{}{}{}{}{}{}};


enum Elements
{
    Hole = 1,
    Goomba = 2,
    KoopaTroopa = 3,
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
    World(vector<vector<int>> baseWorld){};
    ~Mario(){};
    public:
        World_Element* my_world;
        void getNext();
    private:
        int current_position;
        void refreshWorld(){};
        void addElement(){};
 };
