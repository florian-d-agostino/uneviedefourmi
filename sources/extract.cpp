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
bool Extract::readFile(const string& fileName) {
    ifstream file(fileName);

    // Check if the file is successfully opened
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << fileName << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue; // Skip empty lines

        size_t posEqual = line.find('=');
        size_t posDash = line.find('-');

        if (posEqual != string::npos) {
            // Line defines the number of ants (e.g., f=50)
            ants = static_cast<short>(stoi(trim(line.substr(posEqual + 1))));
        }
        else if (posDash != string::npos) {
            // Line defines a connection between two rooms (e.g., RoomA - RoomB)
            string src = trim(line.substr(0, posDash));
            string dest = trim(line.substr(posDash + 1));
            
            // Add the bidirectional edge to the adjacency list map
            connections[src].push_back(dest);
            connections[dest].push_back(src);

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
            size_t posBraceOpen = line.find('{');
            size_t posBraceClose = line.find('}');
            
            string roomName;
            short capacity = 1; // Default capacity is 1

            if (posBraceOpen != string::npos && posBraceClose != string::npos && posBraceClose > posBraceOpen) {
                roomName = trim(line.substr(0, posBraceOpen));
                string capStr = trim(line.substr(posBraceOpen + 1, posBraceClose - posBraceOpen - 1));
                capacity = static_cast<short>(stoi(capStr));
            }
            else {
                roomName = line;
            }

            // Register the room and its capacity in the map
            rooms[roomName] = capacity;
        }
    }
    
    file.close();
    return true;
}

// Get the total number of ants
short Extract::getants() const { return ants; }

// Get the map of rooms and their capacities
const unordered_map<string, short>& Extract::getRooms() const { 
    return rooms; 
}

// Get the adjacency list representing the connections
const unordered_map<string, vector<string>>& Extract::getConnections() const { 
    return connections; 
}
