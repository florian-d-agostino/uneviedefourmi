#include <iostream>
#include "../headers/room.hpp"


using namespace std;




// Constructor
Room::Room(const string& name, short Antmax) {
    this->name = name;
    this->Antmax = Antmax;
    this->Ant = 0;
}





// Getters for Haik
string Room::getName() const {
    return name;
}

short Room::getAntmax() const {
    return Antmax;
}

short Room::getAnt() const {
    return Ant;
}

const vector<Room*>& Room::getDirection() const {
    return Direction;
}







// Link rooms
void Room::linkRoom(Room* neighborRoom) {
    if (neighborRoom != nullptr) {
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
    const unordered_map<string, short>& rooms,
    const unordered_map<string, vector<string>>& connexions
) {
    unordered_map<string, Room*> roomGraph;


    // Create rooms with max capacity
    for (const auto& pair : rooms) {
        const string& name = pair.first;
        short capacity = pair.second;
        roomGraph[name] = new Room(name, capacity);
    }


    // Link rooms according to connections
    for (const auto& pair : connexions) {
        const string& sourceName = pair.first;
        const vector<string>& destinations = pair.second;

        auto sourceIt = roomGraph.find(sourceName);
        if (sourceIt != roomGraph.end()) {
            Room* sourceRoom = sourceIt->second;
            for (const string& destName : destinations) {
                auto destIt = roomGraph.find(destName);
                if (destIt != roomGraph.end()) {

                    sourceRoom->linkRoom(destIt->second);
                }
            }
        }
    }

    return roomGraph;
}




void deleteRoomGraph(unordered_map<string, Room*>& roomGraph) {
    for (auto& pair : roomGraph) {
        delete pair.second;
    }
    roomGraph.clear();
}

