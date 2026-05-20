#include <iostream>
#include "../headers/room.hpp"


using namespace std;


// Constructor
Room::Room(const string& name, int Antmax) {
    this->name = name;
    this->Antmax = Antmax;
    this->Ant = 0;
}

// Getters for Haik
string Room::getName() const {
    return name;
}

int Room::getAntmax() const {
    return Antmax;
}

int Room::getAnt() const {
    return Ant;
}

const vector<Room*>& Room::getDirection() const {
    return Direction;
}




// Link rooms
void Room::linkRoom(Room* neighborRoom) {
    if (neighborRoom != nullptr) {  // check if room does not exist
        Direction.push_back(neighborRoom);
    }
}

// Enter function
bool Room::enter() {
    if (isFull()) {
        return false;
    }
    Ant++;
    cout << name << " contient " << Ant << " fourmis" << endl;
    return true;
}

// Leave function
void Room::leave() {
    if (Ant > 0) {
        Ant--;
        cout << name << " contient " << Ant << " fourmis" << endl;
    }
}

// Check if room is full
bool Room::isFull() const {
    return Ant >= Antmax;
}

// Function to build the room graph
unordered_map<string, Room*> buildRoomGraph(
    const unordered_map<string, int>& dico1,
    const unordered_map<string, vector<string>>& dico2
) {
    unordered_map<string, Room*> rooms;

    // Create rooms with max capacity
    for (const auto& pair : dico1) {
        const string& name = pair.first;
        int capacity = pair.second;
        rooms[name] = new Room(name, capacity);
    }

    // Link rooms according to connections
    for (const auto& pair : dico2) {
        const string& sourceName = pair.first;
        const vector<string>& destinations = pair.second;

        auto sourceIt = rooms.find(sourceName);
        if (sourceIt != rooms.end()) {
            Room* sourceRoom = sourceIt->second;
            for (const string& destName : destinations) {
                auto destIt = rooms.find(destName);
                if (destIt != rooms.end()) {

                    sourceRoom->linkRoom(destIt->second);
                }
            }
        }
    }

    return rooms;
}

