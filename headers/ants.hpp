#ifndef ANTS_HPP
#define ANTS_HPP

#include <vector>
#include "algorithm.hpp"

// Simulates turn-by-turn ant movement using room capacity counters (+1 / -1)
void simulateAntsMovement(Room* start, Room* end, const std::vector<std::vector<Room*>>& routes, int totalAnts);

#endif
