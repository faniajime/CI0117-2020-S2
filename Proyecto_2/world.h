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
    Coin = 4
};


class World
{  
World(vector<vector<int>> baseWorld){};
~World(){};
public:
    
    vector<int> getNext();
private:
    vector<vector<int>> myWorld;
    int world_size;
    int current_position;
    void refreshWorld(){};
    void addElement(){};
};
