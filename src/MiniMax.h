//
// Created by paul on 20/11/18.
//

#ifndef MINIMAX_MINIMAX_H
#define MINIMAX_MINIMAX_H

#include "state.h"
using namespace state;
using namespace std;
unsigned int typedef uint;
namespace MiniMax {

    enum ActionType{
        MOVE_CL= 0,
        MOVE_AW,
        ATTACK,
        HEAL,
    };

    enum MinMax{
        MIN,
        MAX
    };

    class Action {
    public:
        virtual ActionType getActionType() = 0;
        virtual Action* clone() = 0;
    };

    class BestAction{
    private:
        ActionType action;
        int cost;
    public:
        BestAction(int cost,ActionType action):cost(cost),action(action) {};
        ActionType getActionType() {return action;};
        int getCost() {return cost;};
    };

    class Generator {
    private:
        shared_ptr<State> state;
        int tour(State,MinMax,int epoch,uint playerId, uint enemyId);
        int computeCost(State&);
        bool checkCase(Position, State&);
    public:
        shared_ptr<Action> compute(State s);
    };



};



#endif //MINIMAX_MINIMAX_H
