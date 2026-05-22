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

    // Run the algorithm and simulation
    Algorithm algo(ant, 0, ant);
    algo.runAndSimulate(start, end);

    // Clean up memory
    deleteRoomGraph(roomGraph);

    return 0;
}
