//
//  PathFinder.cpp
//  pathFindDemo
//
//  Created by fychen on 10/16/14.
//
//

#include "PathFinder.h"
#include <queue> //std::priority_queue
#include <functional> //std::greater
#include <vector>
#include <unordered_map>
#include <stdlib.h>

using namespace std;

/*(int PathFinder::manhattanHeuristic(int id) {
    auto coord=id2Coord(id);
    auto goalCoord=id2Coord(goal);
    return abs(coord[0]-goalCoord[0])+abs(coord[1]-goalCoord[1]);
}

void PathFinder::runAStar() {
    priority_queue<pair<int, int>, vector<pair<int, int> >, PathFinder::costComparator(true)> frontier;
    frontier.push(make_pair(start, 0));
    unordered_map<int, int> cameFrom;
    cameFrom[start] = -1;
    unordered_map<int, int> costSoFar;
    costSoFar[start] = 0;
    while(!frontier.empty()) {
        auto curr=frontier.top();
        frontier.pop();
        
        if(curr.first==goal) break;
        
        for(const auto& e: edges[curr.first]) {
            if(e.first!=-1 && e.second!=-1) {
                auto newCost=costSoFar[curr.first]+e.second;
                if(costSoFar.count(e.first)==0 || newCost<costSoFar[e.first]) {
                    costSoFar[e.first]=newCost;
                    auto priority=newCost+manhattanHeuristic(e.first);
                    frontier.push(make_pair(e.first, priority));
                    cameFrom[e.first]=curr.first;
                }
            }
        }
    }
}
*/
void PathFinder::init() {
    initEdges();
    //Set walls before calling any specific path finder algorithm
}

void PathFinder::initEdges() {
    edges=vector<vector<pair<int,int> > >(N*N, vector<pair<int, int> >(8, make_pair(-1, -1)));
    wallFlag=vector<bool>(N*N, false);
    //Set the boundary to wall
    for(int i=0;i<N;++i) {
        wallFlag[i]=true;
        wallFlag[i+(N-1)*N]=true;
    }
    for(int i=0;i<N;++i) {
        wallFlag[i*N]=true;
        wallFlag[i*N+N-1]=true;
    }
}

void PathFinder::setWallFlagEntry(int id, bool flag) {
    wallFlag[id]=flag;
}

void PathFinder::genAdjList() {
    for(int i=0;i<N*N;++i) {
        auto coord=id2Coord(i);
        for(int k=0;k<8;++k) {
            vector<int> nextCoord;
            nextCoord.push_back(coord[0]+dirs[k][0]);
            nextCoord.push_back(coord[1]+dirs[k][1]);
            auto nextId=coord2Id(nextCoord);
            edges[i][k].first=nextId;
            if(wallFlag[i] || nextId==-1 || wallFlag[nextId]) {
                edges[i][k].second=-1;
            } else {
                edges[i][k].second=1;
            }
        }
    }
}

vector<int> PathFinder::id2Coord(int id) {
    vector<int> ret;
    ret.push_back(id%N);
    ret.push_back(id/N);
    return ret;
}

int PathFinder::coord2Id(vector<int> coord) {
    if(!isValidCoord(coord)) return -1;
    return coord[0]+coord[1]*N;
}

bool PathFinder::isValidCoord(vector<int> coord) {
    return coord[0]>=0 && coord[1]<N && coord[1]>=0 && coord[1]<N;
}

