#ifndef ROOM_MOCK_HPP
#define ROOM_MOCK_HPP

#include <string>
#include <vector>

// Mock of the Room class to keep the Algorithm part self-contained for testing
struct Room {
    std::string name;
    int antmax;
    int ant = 0;
    std::vector<Room*> exits;
    bool isfull = false;

    Room(std::string name, int antmax) : name(name), antmax(antmax) {}

    void addDirection(Room* room) {
        if (room != nullptr) {
            exits.push_back(room);
        }
    }
};

#endif
