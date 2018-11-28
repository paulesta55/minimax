//
// Created by paul on 20/11/18.
//

#include "MiniMax.h"
#include "AStar.hpp"
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
                    Position objectif = s.getPlayers()[enemyId]->getPokemon()->getPosition();
                    Position current = s.getPlayers()[player]->getPokemon()->getPosition();

                    AStar::Generator generator;
                    int width = (int)(s.getMap()->getWidth());
                    int height = (int)(s.getMap()->getHeight());
                    generator.setWorldSize({(int)(s.getMap()->getWidth()), (int)(s.getMap()->getHeight())});
                    generator.setHeuristic(AStar::Heuristic::manhattan);
                    int k = 0;
                    for (int i =0 ; i<height; i++ )
                    {
                        for (int j = 0 ; j<width; j++)
                        {
                            if(s.getMap()->getLayers()->at(0).getData()->at(k)!=35)
                            {
                                Vec2i v;
                                v.x = j;
                                v.y = i;
                                generator.addCollision(v);
                            }
                            k++;
                        }
                    }
                    Vec2i srce,obj;
                    srce.x = current.x;
                    srce.y = current.y;

                    obj.x = objectif.x;
                    obj.y = objectif.y;
                    auto path = generator.findPath(srce,obj);
                    auto nextTile = path[path.size()-2];
                    cout << nextTile.x << " " << nextTile.y <<endl;
                    Position p(nextTile.x,nextTile.y)
                    s.getPlayers()[playerId]->getPokemon()->setPosition(p);

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
