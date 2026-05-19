#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>
#include "room_mock.hpp" // Switch to the real "room.hpp" when ready

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
    std::vector<Room*> findShortestPath(Room* start, Room* end);

    // Finds multiple optimized and disjoint routes using successive Dijkstra runs
    std::vector<std::vector<Room*>> findOptimizedRoutes(Room* start, Room* end);

    // Dispatches ants onto the optimal routes and simulates turn-by-turn movements
    void dispatchAndSimulate(Room* start, Room* end);
};

#endif
