//
//  AStarFinder.h
//  pathFindDemo
//
//  Created by fychen on 10/16/14.
//
//

#ifndef __pathFindDemo__AStarFinder__
#define __pathFindDemo__AStarFinder__

#include <iostream>
#include <vector>
#include <utility>
#include <queue> //std::priority_queue
#include <functional> //std::greater
#include <unordered_map>
#include <map>
#include <stdlib.h>

#include "PathFinder.h"

using namespace std;

class AStarFinder : public PathFinder{
public:
    class costComparator{
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
    };
    typedef std::priority_queue<pair<int,int> ,std::vector<pair<int, int> >,costComparator> pqType;
private:
    bool _isFin;
    pqType frontier;
    unordered_map<int, int> cameFrom;
    unordered_map<int, int> costSoFar;
    map<int, int> _config; //first-->id, second-->type[0->inner, 1->frontier]
public:
    AStarFinder(int n):PathFinder(n) {}
    virtual void init();
    int manhHeuristic(int id);
    void expandFrontier();
    inline bool isFin() {return _isFin;}
    
    inline const map<int, int>& getConfig() const {return _config;}
};

#endif /* defined(__pathFindDemo__AStarFinder__) */
