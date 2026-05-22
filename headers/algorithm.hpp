#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>
#include "room.hpp"
#include <unordered_set>
#include <set>
#include <utility>

class Algorithm {
private:
    int StartAnt;
    int EndAnt;
    int initialMax;

public:
    // Constructor
    Algorithm(int startAnt, int endAnt, int initialMax);

    // Getters and Setters
    int getStartAnt() const;
    int getEndAnt() const;
    int getInitialMax() const;
    
    void setStartAnt(int count);
    void setEndAnt(int count);

    // Finds a single shortest path using classic Dijkstra
    std::vector<Room*> findShortestPath(
        Room* start, 
        Room* end, 
        const std::unordered_set<Room*>& blockedRooms = {}, 
        const std::set<std::pair<Room*, Room*>>& blockedEdges = {}
    );

    // Finds multiple optimized and disjoint routes using successive Dijkstra runs
    std::vector<std::vector<Room*>> findOptimizedRoutes(Room* start, Room* end);

    // Dispatches ants onto the optimal routes and simulates turn-by-turn movements
    void dispatchAndSimulate(Room* start, Room* end);

    // Runs shortest path, optimized routes, and simulates turn-by-turn movement
    void runAndSimulate(Room* start, Room* end);
};

#endif
