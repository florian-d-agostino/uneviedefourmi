#include <iostream>
#include "extract.cpp"
#include "../headers/room.hpp"

int main() {
    Extract extracteur;
    extracteur.lireFichier("ressources/fourmilieres_texte/fourmiliere_0.txt");    
    short ant = extracteur.getants();
    const auto& rooms = extracteur.getRooms();
    const auto& connexions = extracteur.getConnexions();

    // Build the room graph
    unordered_map<string, Room*> roomGraph = buildRoomGraph(rooms, connexions);

    std::cout << "Graphe construit avec " << roomGraph.size() << " salles." << std::endl;

    // Clean up memory
    deleteRoomGraph(roomGraph);

    return 0;
}
