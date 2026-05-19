#include <iostream>
#include <vector>
#include "../headers/room_mock.hpp"
#include "../headers/algorithm.hpp"
#include "../headers/ants.hpp"

int main() {
    std::cout << "--- SIMULATION WITH MOCK DATA ---" << std::endl;

    // 1. Create mock rooms
    Room* start = new Room("Sv", 1);
    Room* end = new Room("Sd", 1);
    Room* s1 = new Room("S1", 1);
    Room* s2 = new Room("S2", 1);

    // 2. Build the connections (bidirectional)
    
    // Direct path: Sv -> Sd
    start->addDirection(end);
    end->addDirection(start);

    // Path 2: Sv -> S1 -> S2 -> Sd
    start->addDirection(s1);
    s1->addDirection(start);

    s1->addDirection(s2);
    s2->addDirection(s1);

    s2->addDirection(end);
    end->addDirection(s2);

    // Number of ants to simulate
    int numAnts = 5;

    std::cout << "Ants count: " << numAnts << "\n" << std::endl;

    // 3. Initialize algorithm
    Algorithm algo(numAnts, 0, numAnts);

    std::cout << "--- SINGLE SHORTEST PATH (Dijkstra) ---" << std::endl;
    std::vector<Room*> path = algo.findShortestPath(start, end);
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i]->name << (i == path.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found." << std::endl;
    }

    std::cout << "\n--- DISJOINT OPTIMIZED ROUTES ---" << std::endl;
    std::vector<std::vector<Room*>> routes = algo.findOptimizedRoutes(start, end);
    if (!routes.empty()) {
        for (size_t r = 0; r < routes.size(); ++r) {
            std::cout << "Route " << (r + 1) << " : ";
            for (size_t i = 0; i < routes[r].size(); ++i) {
                std::cout << routes[r][i]->name << (i == routes[r].size() - 1 ? "" : " -> ");
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No routes found." << std::endl;
    }

    std::cout << std::endl;

    // 4. Simulate movement
    simulateAntsMovement(start, end, routes, numAnts);

    // Free memory
    delete start;
    delete end;
    delete s1;
    delete s2;

    return 0;
}
