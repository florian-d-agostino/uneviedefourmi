#include <iostream>
#include <vector>
#include <unordered_map>
#include "extract.cpp"
#include "../headers/room.hpp"
#include "../headers/algorithm.hpp"
#include "../headers/ants.hpp"

int main(int argc, char* argv[]) {
    // Determine which file to load
    std::string filePath = "ressources/fourmilieres_texte/fourmiliere_0.txt";
    if (argc > 1) {
        filePath = argv[1];
    }

    std::cout << "Loading file: " << filePath << std::endl;

    Extract extracteur;
    if (!extracteur.lireFichier(filePath)) {
        std::cerr << "Erreur de lecture du fichier" << std::endl;
        return 1;
    }    
    
    short ant = extracteur.getants();
    const auto& rooms = extracteur.getRooms();
    const auto& connexions = extracteur.getConnexions();

    std::cout << "Nombre de fourmis : " << ant << std::endl;

    // Build the room graph
    std::unordered_map<std::string, Room*> roomGraph = buildRoomGraph(rooms, connexions);

    std::cout << "Graphe construit avec " << roomGraph.size() << " salles." << std::endl;

    // Retrieve the start ("Sv") and end ("Sd") rooms
    auto startIt = roomGraph.find("Sv");
    auto endIt = roomGraph.find("Sd");
    if (startIt == roomGraph.end() || endIt == roomGraph.end()) {
        std::cerr << "Erreur : Salles de depart (Sv) ou d'arrivee (Sd) introuvables." << std::endl;
        deleteRoomGraph(roomGraph);
        return 1;
    }
    Room* start = startIt->second;
    Room* end = endIt->second;

    // Run the algorithm
    Algorithm algo(ant, 0, ant);

    std::cout << "\n--- SINGLE SHORTEST PATH (Dijkstra) ---" << std::endl;
    std::vector<Room*> path = algo.findShortestPath(start, end);
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i]->getName() << (i == path.size() - 1 ? "" : " -> ");
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
                std::cout << routes[r][i]->getName() << (i == routes[r].size() - 1 ? "" : " -> ");
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No routes found." << std::endl;
    }

    std::cout << std::endl;

    // Simulate movement
    simulateAntsMovement(start, end, routes, ant);

    // Clean up memory
    deleteRoomGraph(roomGraph);

    return 0;
}
