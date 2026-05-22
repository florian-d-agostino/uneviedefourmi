#include "../headers/algorithm.hpp"
#include "../headers/ants.hpp"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iostream>

Algorithm::Algorithm(int startAnt, int endAnt, int initialMax)
    : StartAnt(startAnt), EndAnt(endAnt), initialMax(initialMax) {}

int Algorithm::getStartAnt() const {
    return StartAnt;
}

int Algorithm::getEndAnt() const {
    return EndAnt;
}

int Algorithm::getInitialMax() const {
    return initialMax;
}

void Algorithm::setStartAnt(int count) {
    StartAnt = count;
}

void Algorithm::setEndAnt(int count) {
    EndAnt = count;
}

// Find the shortest path using Dijkstra's algorithm
std::vector<Room*> Algorithm::findShortestPath(
    Room* start, 
    Room* end, 
    const std::unordered_set<Room*>& blockedRooms, 
    const std::set<std::pair<Room*, Room*>>& blockedEdges
) {
    if (start == nullptr || end == nullptr) {
        return {};
    }

    // Stores the minimum distance from start to each room
    std::unordered_map<Room*, int> dists;
    
    // Stores the predecessor of each room to reconstruct the path
    std::unordered_map<Room*, Room*> parent;

    // Min-priority queue storing pairs of (distance, Room*)
    typedef std::pair<int, Room*> Element;
    std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;

    // Initialize source room
    dists[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        Room* current = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        // Skip if a shorter path to this room is already found
        if (current_dist > dists[current]) {
            continue;
        }

        // Stop if destination is reached
        if (current == end) {
            break;
        }

        // Explore neighbors
        for (Room* neighbor : current->getDirection()) {
            // Ignore blocked rooms unless it is the destination
            if (blockedRooms.find(neighbor) != blockedRooms.end() && neighbor != end) {
                continue;
            }

            // Ignore blocked edges
            if (blockedEdges.find({current, neighbor}) != blockedEdges.end()) {
                continue;
            }

            // Each transition between rooms has a weight of 1
            int new_dist = current_dist + 1;

            if (dists.find(neighbor) == dists.end() || new_dist < dists[neighbor]) {
                dists[neighbor] = new_dist;
                parent[neighbor] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }

    // Return empty if destination is unreachable
    if (dists.find(end) == dists.end()) {
        return {};
    }

    // Reconstruct the path backwards from end to start
    std::vector<Room*> path;
    Room* curr = end;
    while (curr != nullptr) {
        path.push_back(curr);
        curr = parent[curr];
    }

    // Reverse path to get correct order (start -> end)
    std::reverse(path.begin(), path.end());
    return path;
}

// Find multiple disjoint optimized routes using successive Dijkstra searches
std::vector<std::vector<Room*>> Algorithm::findOptimizedRoutes(Room* start, Room* end) {
    std::vector<std::vector<Room*>> routes;
    std::unordered_set<Room*> blockedRooms;
    std::set<std::pair<Room*, Room*>> blockedEdges;

    while (true) {
        // Find the current shortest path
        std::vector<Room*> path = findShortestPath(start, end, blockedRooms, blockedEdges);
        if (path.empty()) {
            break; // No more paths available
        }

        routes.push_back(path);

        if (path.size() == 2) {
            // Temporarily block the direct link to avoid infinite loop
            blockedEdges.insert({start, end});
            blockedEdges.insert({end, start});
        } else {
            // Temporarily block rooms on this path (except start and end) for subsequent searches
            for (size_t i = 1; i < path.size() - 1; ++i) {
                blockedRooms.insert(path[i]);
            }
        }
    }

    return routes;
}

// Dispatch ants onto optimal routes and simulate turn-by-turn movements
void Algorithm::dispatchAndSimulate(Room* start, Room* end) {
    // 1. Find all available disjoint routes
    std::vector<std::vector<Room*>> routes = findOptimizedRoutes(start, end);
    if (routes.empty()) {
        std::cout << "No routes available for the ants!" << std::endl;
        return;
    }

    // 2. Distribute ants across routes
    // antsOnRoute[i] holds the IDs of ants assigned to route i
    std::vector<std::vector<int>> antsOnRoute(routes.size());
    
    for (int antId = 1; antId <= StartAnt; ++antId) {
        int bestRouteIdx = 0;
        int minTime = 1e9;
        
        // Find the route that minimizes estimated arrival time: Route Length + Ants Already Assigned
        for (size_t i = 0; i < routes.size(); ++i) {
            int routeLength = routes[i].size() - 1; // transitions count
            int currentAnts = antsOnRoute[i].size();
            int estimatedTime = routeLength + currentAnts;
            
            if (estimatedTime < minTime) {
                minTime = estimatedTime;
                bestRouteIdx = i;
            }
        }
        
        antsOnRoute[bestRouteIdx].push_back(antId);
    }

    // Output dispatch distribution
    std::cout << "--- ANT DISTRIBUTION ---" << std::endl;
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "Route " << (i + 1) << " (length " << (routes[i].size() - 1) << ") gets " 
                  << antsOnRoute[i].size() << " ants: [ ";
        for (int id : antsOnRoute[i]) {
            std::cout << "L" << id << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;

    // 3. Turn-by-turn simulation
    std::cout << "--- SIMULATION MOVEMENT ---" << std::endl;
    
    // positions[i] tracks the current index of each ant on route i
    std::vector<std::vector<int>> positions(routes.size());
    for (size_t i = 0; i < routes.size(); ++i) {
        positions[i].resize(antsOnRoute[i].size(), 0); // All start at index 0 (start room)
    }

    int turn = 1;
    bool finished = false;
    std::vector<size_t> nextAntToStart(routes.size(), 0);

    while (!finished) {
        finished = true;
        std::cout << "Turn " << turn << " : ";
        bool firstPrint = true;

        for (size_t r = 0; r < routes.size(); ++r) {
            // Move ants already in transit
            int numAnts = antsOnRoute[r].size();
            for (int a = 0; a < numAnts; ++a) {
                int currentPos = positions[r][a];
                
                if (currentPos > 0 && currentPos < (int)routes[r].size() - 1) {
                    positions[r][a]++;
                    int antId = antsOnRoute[r][a];
                    std::string roomName = routes[r][positions[r][a]]->getName();
                    
                    if (!firstPrint) std::cout << ", ";
                    std::cout << "L" << antId << "-" << roomName;
                    firstPrint = false;
                }
            }

            // Launch a new ant on this route if possible
            size_t nextIdx = nextAntToStart[r];
            if (nextIdx < antsOnRoute[r].size()) {
                positions[r][nextIdx] = 1;
                int antId = antsOnRoute[r][nextIdx];
                std::string roomName = routes[r][1]->getName();
                
                if (!firstPrint) std::cout << ", ";
                std::cout << "L" << antId << "-" << roomName;
                firstPrint = false;
                
                nextAntToStart[r]++;
            }
            
            // Check if there are still ants in transit or waiting to start
            for (int a = 0; a < numAnts; ++a) {
                if (positions[r][a] < (int)routes[r].size() - 1) {
                    finished = false;
                }
            }
        }

        if (finished) {
            std::cout << "(All ants have arrived)" << std::endl;
        } else {
            std::cout << std::endl;
            turn++;
        }
    }
    
    std::cout << "\nTotal turns required: " << (turn - 1) << " turns." << std::endl;
}

void Algorithm::runAndSimulate(Room* start, Room* end) {
    std::cout << "\n--- SINGLE SHORTEST PATH (Dijkstra) ---" << std::endl;
    std::vector<Room*> path = findShortestPath(start, end);
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i]->getName() << (i == path.size() - 1 ? "" : " -> ");
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found." << std::endl;
    }

    std::cout << "\n--- DISJOINT OPTIMIZED ROUTES ---" << std::endl;
    std::vector<std::vector<Room*>> routes = findOptimizedRoutes(start, end);
    if (!routes.empty()) {
        for (size_t r = 0; r < routes.size(); ++r) {
            std::cout << "Route " << (r + 1) << " : ";
            for (size_t i = 0; i < routes[r].size(); ++i) {
                std::cout << routes[r][i]->getName() << (i == routes[r].size() - 1 ? "" : " -> ");
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No routes found." << std::endl;
    }

    std::cout << std::endl;

    // Simulate movement
    simulateAntsMovement(start, end, routes, StartAnt);
}