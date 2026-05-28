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

void Room::setAnt(short count) {
    Ant = count;
}

void Room::setAntmax(short max) {
    Antmax = max;
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
    cout << name << " contains " << Ant << " ants" << endl;
    return true;
}



// Leave function
void Room::leave() {
    if (Ant > 0) {
        Ant--;
        cout << name << " contains " << Ant << " ants" << endl;
    }
}



// Check if room is full
bool Room::isFull() const {
    return Ant >= Antmax;
}





