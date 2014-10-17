//
//  PathFinder.h
//  pathFindDemo
//
//  Created by fychen on 10/16/14.
//
//

#ifndef __pathFindDemo__PathFinder__
#define __pathFindDemo__PathFinder__

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class PathFinder {
    /*class costComparator{
        bool reverse;
    public:
        costComparator(const bool& rev=false) {reverse=rev;}
        bool operator() (const pair<int, int>& lhs, const pair<int, int>& rhs) const {
            if(reverse) {
                return (lhs.second>rhs.second);
            } else {
                return (lhs.second<lhs.second);
            }
        }
    };*/
public:
    vector<vector<int> > dirs={
        {0,-1},
        {1,0},
        {0,1},
        {-1,0},
        {-1,-1},
        {1,-1},
        {1,1},
        {-1,1}
    };
    int N=0;
    vector<vector<pair<int ,int> > > edges;
    int start;
    int goal;
    vector<bool> wallFlag;
public:
    
    PathFinder(int n) {N=n;}
    
    virtual void init();
    
    inline void setN(int n) {N=n;}
    inline int getN() {return N;}
    
    inline void setStart(int st) {start=st;}
    inline int getStart() {return start;}
    
    inline void setGoal(int gl) {goal=gl;}
    inline int getGoal() {return goal;}
    
    void setWallFlagEntry(int id, bool flag);
    
    int coord2Id(vector<int> coor);
    vector<int> id2Coord(int id);
    
    bool isValidCoord(vector<int> coord);
    
    int manhattanHeuristic(int id);
    void genAdjList();
private:
    void initEdges();
    
};


#endif /* defined(__pathFindDemo__PathFinder__) */
