//
// Created by paul on 20/11/18.
//

#include "MiniMax.h"
#include <climits>
using namespace MiniMax;



int Generator::tour(State s, MinMax m, int epoch, uint playerId, uint enemyId) {
    if(epoch==0) {
        return this->computeCost(s);
    }
    else {
        int cost = m == MAX?0:INT_MAX;
        for(int i = ActionType ::MOVE_CL; i< ActionType::ATTACK;i++) {
            State newState = s;
            switch(i) {
                case ActionType::MOVE_CL :
                    // TODO: use heuristic ai to compute newState
                    break;
                case ActionType::MOVE_AW :
                    // TODO: find algo which computes the best move to avoid the other player and compute newState
                    break;
                case ActionType ::ATTACK :
                    // TODO: make a method which compute whether player can attack enemy
                    // if player can attack then enemyPV-=1 else do nothing
                    break;
                case ActionType ::HEAL :
                    // if player wounded playerPV+=1 else do nothing
                    int playerPV = newState.getPlayers()[playerId]->getPokemon()->getCurrentLife();
                    // TODO: create a heal abstract method in Pokemon
                    break;
            }
            epoch--
            if(m == MinMax::MAX) {
                int tempCost = tour(newState,MIN,epoch,enemyId,playerId);
                cost = tempCost > cost?tempCost:cost;
            }else {
                int tempCost = tour(newState,MAX,epoch,enemyId,playerId);
                cost = tempCost < cost?tempCost:cost;
            }
        }
        return cost;
    }
}

int Generator::computeCost(State &) {
    // TODO: implement heuristic formula
    return 0;
}
