//
// Created by paul on 20/11/18.
//

#include "MiniMax.h"
#include "AStar.hpp"
#include "engine.h"
#include <climits>
#include <cmath>
using namespace MiniMax;

using namespace engine;


int Generator::tour(State s, MinMax m, int epoch, uint playerId, uint enemyId) {
    if(epoch==0) {
        return this->computeCost(s);
    }
    else {
        int cost = m == MAX?0:INT_MAX;
        for(int i = ActionType ::MOVE_CL; i< ActionType::ATTACK;i++) {
            State newState = s;
            Engine engine(s);
            switch(i) {
                case ActionType::MOVE_CL :
                    // use AStar AI
                    Position objectif = engine.getState().getPlayers()[enemyId]->getPokemon()->getPosition();
                    Position current = engine.getState().getPlayers()[player]->getPokemon()->getPosition();

                    AStar::Generator generator;
                    int width = (int)(engine.getState().getMap()->getWidth());
                    int height = (int)(engine.getState().getMap()->getHeight());
                    generator.setWorldSize({(int)(engine.getState().getMap()->getWidth()), (int)(engine.getState().getMap()->getHeight())});
                    generator.setHeuristic(AStar::Heuristic::manhattan);
                    int k = 0;
                    for (int i =0 ; i<height; i++ )
                    {
                        for (int j = 0 ; j<width; j++)
                        {
                            if(engine.getState().getMap()->getLayers()->at(0).getData()->at(k)!=35)
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
                    engine.getState().getPlayers()[playerId]->getPokemon()->setPosition(p);

                    break;
                case ActionType::MOVE_AW :
                    // go to enemy orientation if possible. If not go to the first possible location
                    Position currentPos = engine.getState().getPlayers()[playerId]->getPokemon()->getPosition();
                    Position enemyPos = engine.getState().getPlayers()[enemyId]->getPokemon()->getPosition();
                    Orientation enemyOrient = engine.getState().getPlayers()[enemyId]->getPokemon()->getOrientation();
                    bool oppositeOrient = false;
                    //check position according to enemyOrient
                    switch(enemyOrient) {
                        case SOUTH:
                            oppositeOrient = checkCase(Position(currentPos.x,currentPos.y+1),engine.getState());
                            break;
                        case EST:
                            oppositeOrient = checkCase(Position(currentPos.x+1,currentPos.y),engine.getState());
                            break;
                        case NORTH:
                            oppositeOrient = checkCase(Position(currentPos.x,currentPos.y-1),engine.getState());
                            break;
                        case WEST:
                            oppositeOrient = checkCase(Position(currentPos.x-1,currentPos.y),engine.getState());
                            break;
                    }
                    if(oppositeOrient) {
                        engine.addCommand(make_shared<MoveCommand>(enemyOrient,playerId));
                    }
                    else {
                        auto neighbors = findNeighbors(currentPos,engine.getState(),enemyPos);
                        engine.addCommand(make_shared<MoveCommand(neighbors[0],playerId));
                    }
                    engine.runCommands();
                    break;
                case ActionType ::ATTACK :
                    engine.addCommand(make_shared<AttackCommand>(playerId));
                    engine.runCommands();
                    // if player can attack then enemyPV-=1 else does nothing
                    break;
                case ActionType ::HEAL :
                    // if player wounded playerPV+=1 else does nothing
                    engine.addCommand(make_shared<HealCommand>(playerId));
                    engine.runCommands();

                    break;
            }
            epoch--
            if(m == MinMax::MAX) {
                int tempCost = tour(engine.getState(),MIN,epoch,enemyId,playerId);
                cost = tempCost > cost?tempCost:cost;
            }else {
                int tempCost = tour(newState,MAX,epoch,enemyId,playerId);
                cost = tempCost < cost?tempCost:cost;
            }
        }
        return cost;
    }
}

int Generator::computeCost(State& s, uint enemyId, uint playerId) {
    Position enemyP=s.getPlayers()[enemyId]->getPokemon()->getPosition();
    Position currentP = s.getPlayers()[playerId]->getPokemon()->getPosition();
    // use manhattan formula * 10
    uint  distance = static_cast<uint >(10*(abs(enemyP.x-currentP.x)+abs(enemyP.y-currentP.y)));
    int cost = s.getPlayers()[playerId]->getPokemon()->getCurrentLife()-s.getPlayers()[enemyId]->getPokemon()->getCurrentLife()-distance;
    return 0;
}

bool Generator::checkCase(Position p, State& s) {
    uint tileNumber = p.x + p.y * s.getMap()->getWidth();
    if(s.getMap()->getLayers()->at(0).getData()[tileNumber]!=35) {
        return true;
    }
    return false;
}

vector<Orientation> Generator::findNeighbors(Position& p, State& s,Position& forbiddP) {
    vector<Orientation> neighbors;
    for(auto o : Orientation) {
        switch(o) {
            case SOUTH:
                Position southP = Position(p.x,p.y+1);
                if(checkCase(southP,s) && southP.x != forbiddP.x && southP.y != forbiddP.y) {
                    neighbors.push_back(SOUTH);
                }
                break;
            case NORTH:
                Position northP = Position(p.x, p.y-1);
                if(checkCase(northP,s) && northP.x != forbiddP.x && northP.y != forbiddP.y) {
                    neighbors.push_back(NORTH);
                }
                break;
            case EST:
                Position estP= Position(p.x+1, p.y);
                if(checkCase(estP,s) && estP.x != forbiddP.x && estP.y != forbiddP.y) {
                    neighbors.push_back(EST);
                }
                break;
            case WEST:
                Position westP= Position(p.x-1, p.y);
                if(checkCase(westP,s) && westP.x != forbiddP.x && westP.y != forbiddP.y) {
                    neighbors.push_back(WEST);
                }
                break;

        }
    }
    return neighbors;
}
