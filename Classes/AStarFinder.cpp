//
//  AStarFinder.cpp
//  pathFindDemo
//
//  Created by fychen on 10/16/14.
//
//

#include "AStarFinder.h"

using namespace std;

int AStarFinder::manhHeuristic(int id) {
    auto coord=id2Coord(id);
    auto goalCoord=id2Coord(goal);
    return abs(coord[0]-goalCoord[0])+abs(coord[1]-goalCoord[1]);
}

void AStarFinder::init() {
    //Super class init.
    PathFinder::init();
    
    
    _isFin=false;
    frontier = pqType(costComparator(true));
    frontier.push(make_pair(start, 0));
    cameFrom[start] = -1;
    costSoFar[start] = 0;
}

void AStarFinder::expandFrontier() {
    if(!frontier.empty()) {
        auto curr=frontier.top();
        frontier.pop();
        //change curr to inner state
        if(_config.count(curr.first)!=0) {
            _config[curr.first]=0;
        }
        
        if(curr.first==goal) {
            _isFin=true;
            return;
        }
        
        for(const auto& e: edges[curr.first]) {
            if(e.first!=-1 && e.second!=-1) {
                auto newCost=costSoFar[curr.first]+e.second;
                if(costSoFar.count(e.first)==0 || newCost<costSoFar[e.first]) {
                    costSoFar[e.first]=newCost;
                    auto priority=newCost+manhHeuristic(e.first);
                    frontier.push(make_pair(e.first, priority));
                    //update config
                    if(_config.count(e.first)==0) {
                        _config[e.first]=1;
                    }
                    cameFrom[e.first]=curr.first;
                }
            }
        }
    }
}
