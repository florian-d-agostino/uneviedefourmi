#include "../headers/extract.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

// Helper function to remove leading and trailing whitespaces from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

Extract::Extract() : ants(0) {}

// Reads the file, parses each line and extracts the ants, rooms, and connections
bool Extract::lireFichier(const string& nomFichier) {
    ifstream fichier(nomFichier);

    // Check if the file is successfully opened
    if (!fichier.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << endl;
        return false;
    }

    string ligne;
    while (getline(fichier, ligne)) {
        ligne = trim(ligne);
        if (ligne.empty()) continue; // Skip empty lines

        size_t posEgal = ligne.find('=');
        size_t posTiret = ligne.find('-');

        if (posEgal != string::npos) {
            // Line defines the number of ants (e.g., f=50)
            ants = static_cast<short>(stoi(trim(ligne.substr(posEgal + 1))));
        }
        else if (posTiret != string::npos) {
            // Line defines a connection between two rooms (e.g., RoomA - RoomB)
            string src = trim(ligne.substr(0, posTiret));
            string dest = trim(ligne.substr(posTiret + 1));
            
            // Add the bidirectional edge to the adjacency list map
            connexions[src].push_back(dest);
            connexions[dest].push_back(src);

            // Initialize implicit rooms in the rooms map if not already present
            // Start (Sv) and End (Sd) rooms have a large "infinite" capacity
            if (rooms.find(src) == rooms.end()) {
                rooms[src] = (src == "Sv" || src == "Sd") ? 32767 : 1;
            }
            if (rooms.find(dest) == rooms.end()) {
                rooms[dest] = (dest == "Sv" || dest == "Sd") ? 32767 : 1;
            }
        }
        else {
            // Line defines a room and optionally its capacity (e.g., S1 { 8 } or S2)
            size_t posAccOuv = ligne.find('{');
            size_t posAccFer = ligne.find('}');
            
            string nomRoom;
            short capacity = 1; // Default capacity is 1

            if (posAccOuv != string::npos && posAccFer != string::npos && posAccFer > posAccOuv) {
                nomRoom = trim(ligne.substr(0, posAccOuv));
                string capStr = trim(ligne.substr(posAccOuv + 1, posAccFer - posAccOuv - 1));
                capacity = static_cast<short>(stoi(capStr));
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
short Extract::getants() const { return ants; }

// Get the map of rooms and their capacities
const unordered_map<string, short>& Extract::getRooms() const { 
    return rooms; 
}

// Get the adjacency list representing the connections
const unordered_map<string, vector<string>>& Extract::getConnexions() const { 
    return connexions; 
}
