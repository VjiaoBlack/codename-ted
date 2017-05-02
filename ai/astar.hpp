#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

// DATA STRUCTURES 
class q_elem {
    public:
        vec2 pos;
        vec2 start;
        float priority;
        q_elem(vec2 a, vec2 b, int c) : 
            pos(a), start(b), priority(c) {};
};

struct PriorityComp
{
    bool operator()(const q_elem& lhs, const q_elem& rhs) const {
        return lhs.priority > rhs.priority;
    }
};

// FUNCTION HEADERS
void astar_main(); 
vector<vec2> retrieve_ship_positions(gameMap gm); 
vector<vec2> retrieve_next_positions(vec2 curr, int max_x, int max_y);
float distance(vec2 pos1, vec2 pos2);
bool q_elem_compare(q_elem a, q_elem b);
vec2 get_target(vector<vec2> positions);
void merchant_moves(gameMap& gm);


