#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

// Helper function to remove leading and trailing whitespaces from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Class to parse and extract information from the labyrinth config file
class Extract {
private:
    int fourmis; // Number of ants
    std::map<std::string, int> rooms; // Map: Room name -> Capacity
    std::map<std::string, std::vector<std::string>> connexions; // Adjacency List: Room name -> Neighboring rooms

public:
    Extract() : fourmis(0) {}

    // Reads the file, parses each line and extracts the ants, rooms, and connections
    bool lireFichier(const std::string& nomFichier) {
        std::ifstream fichier(nomFichier);

        // Check if the file is successfully opened
        if (!fichier.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << std::endl;
            return false;
        }

        std::string ligne;
        while (std::getline(fichier, ligne)) {
            ligne = trim(ligne);
            if (ligne.empty()) continue; // Skip empty lines

            size_t posEgal = ligne.find('=');
            size_t posTiret = ligne.find('-');

            if (posEgal != std::string::npos) {
                // Line defines the number of ants (e.g., f=50)
                fourmis = std::stoi(trim(ligne.substr(posEgal + 1)));
            }
            else if (posTiret != std::string::npos) {
                // Line defines a connection between two rooms (e.g., RoomA - RoomB)
                std::string src = trim(ligne.substr(0, posTiret));
                std::string dest = trim(ligne.substr(posTiret + 1));
                
                // Add the bidirectional edge to the adjacency list map
                connexions[src].push_back(dest);
                connexions[dest].push_back(src);

                // Initialize implicit rooms in the rooms map if not already present
                // Start (Sv) and End (Sd) rooms have a large "infinite" capacity
                if (rooms.find(src) == rooms.end()) {
                    rooms[src] = (src == "Sv" || src == "Sd") ? 999999 : 1;
                }
                if (rooms.find(dest) == rooms.end()) {
                    rooms[dest] = (dest == "Sv" || dest == "Sd") ? 999999 : 1;
                }
            }
            else {
                // Line defines a room and optionally its capacity (e.g., S1 { 8 } or S2)
                size_t posAccOuv = ligne.find('{');
                size_t posAccFer = ligne.find('}');
                
                std::string nomRoom;
                int capacity = 1; // Default capacity is 1

                if (posAccOuv != std::string::npos && posAccFer != std::string::npos && posAccFer > posAccOuv) {
                    nomRoom = trim(ligne.substr(0, posAccOuv));
                    std::string capStr = trim(ligne.substr(posAccOuv + 1, posAccFer - posAccOuv - 1));
                    capacity = std::stoi(capStr);
                }
                else {
                    nomRoom = ligne;
                }

                // Register the room and its capacity in the map
                rooms[nomRoom] = capacity;
            }
        }
        
        fichier.close();
        return true;
    }

    // Get the total number of ants
    int getFourmis() const { return fourmis; }
    
    // Get the map of rooms and their capacities
    const std::map<std::string, int>& getRooms() const { 
        return rooms; 
    }
    
    // Get the adjacency list representing the connections
    const std::map<std::string, std::vector<std::string>>& getConnexions() const { 
        return connexions; 
    }
};
