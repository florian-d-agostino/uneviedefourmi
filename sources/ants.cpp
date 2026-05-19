#include "../headers/ants.hpp"
#include <iostream>

// Simulates turn-by-turn ant movement using room capacity counters (+1 / -1)
void simulateAntsMovement(Room* start, Room* end, const std::vector<std::vector<Room*>>& routes, int totalAnts) {
    if (routes.empty() || start == nullptr || end == nullptr) {
        std::cout << "Invalid simulation parameters." << std::endl;
        return;
    }

    // 1. Calculate the optimal number of ants to assign to each route.
    // This greedy distribution ensures we only use a route if it actually saves time.
    std::vector<int> antsLimitPerRoute(routes.size(), 0);
    for (int a = 0; a < totalAnts; ++a) {
        int bestRouteIdx = 0;
        int minTime = 1e9;
        for (size_t i = 0; i < routes.size(); ++i) {
            int routeLength = routes[i].size() - 1; // number of transitions
            int currentAnts = antsLimitPerRoute[i];
            int estimatedTime = routeLength + currentAnts;
            if (estimatedTime < minTime) {
                minTime = estimatedTime;
                bestRouteIdx = i;
            }
        }
        antsLimitPerRoute[bestRouteIdx]++;
    }

    // Set initial counts and capacities
    start->ant = totalAnts;
    start->antmax = 1e9; // Start has infinite capacity
    end->ant = 0;
    end->antmax = 1e9;   // End has infinite capacity

    // Initialize all intermediate rooms to 0 ants
    for (const auto& route : routes) {
        for (size_t i = 1; i < route.size() - 1; ++i) {
            route[i]->ant = 0;
        }
    }

    std::vector<int> antsLaunched(routes.size(), 0);
    int turn = 1;
    
    std::cout << "--- SIMULATION MOVEMENT (COUNTER-BASED) ---" << std::endl;

    while (end->ant < totalAnts) {
        std::cout << "Turn " << turn << " : ";
        bool movedThisTurn = false;

        // Process each route to move ants forward
        for (size_t r = 0; r < routes.size(); ++r) {
            const auto& route = routes[r];
            // Process backwards (from end-1 down to start) to enable pipeline flow
            for (int i = static_cast<int>(route.size()) - 2; i >= 0; --i) {
                Room* current = route[i];
                Room* next = route[i + 1];

                if (i == 0) {
                    // Moving from the start room: check the optimal launch limit for this route
                    if (current->ant > 0 && antsLaunched[r] < antsLimitPerRoute[r] && (next == end || next->ant < next->antmax)) {
                        current->ant--;
                        next->ant++;
                        antsLaunched[r]++;
                        movedThisTurn = true;
                        std::cout << "[" << current->name << " -> " << next->name << "] ";
                    }
                } else {
                    // Moving from an intermediate room
                    if (current->ant > 0 && (next == end || next->ant < next->antmax)) {
                        current->ant--;
                        next->ant++;
                        movedThisTurn = true;
                        std::cout << "[" << current->name << " -> " << next->name << "] ";
                    }
                }
            }
        }

        if (!movedThisTurn && end->ant < totalAnts) {
            std::cout << "STUCK! (No valid moves possible)" << std::endl;
            break;
        }

        std::cout << "(Ants at destination: " << end->ant << "/" << totalAnts << ")" << std::endl;
        turn++;
    }

    std::cout << "\nTotal turns: " << (turn - 1) << std::endl;
}
