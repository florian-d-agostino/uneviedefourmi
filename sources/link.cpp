#include "../headers/link.hpp"
#include "../headers/extract.hpp"
#include <iostream>

Link::Link() : sv(nullptr), sd(nullptr) {}

Link::~Link() {
    for (auto& pair : rooms) {
        delete pair.second;
    }
    rooms.clear();
}

void Link::addroom(Room* room) {
    if (room == nullptr) return;
    std::string name = room->getName();
    rooms[name] = room;
    if (name == "Sv") {
        sv = room;
    } else if (name == "Sd") {
        sd = room;
    }
}

void Link::addlink(const std::string& nameS1, const std::string& nameS2) {
    auto it1 = rooms.find(nameS1);
    auto it2 = rooms.find(nameS2);
    if (it1 != rooms.end() && it2 != rooms.end()) {
        it1->second->linkRoom(it2->second);
    }
}

Room* Link::getSv() const {
    return sv;
}

Room* Link::getSd() const {
    return sd;
}

const std::unordered_map<std::string, Room*>& Link::getRooms() const {
    return rooms;
}

void Link::buildFromExtract(const Extract& extract) {
    // 1. Create all rooms
    for (const auto& pair : extract.getRooms()) {
        addroom(new Room(pair.first, pair.second));
    }

    // 2. Add all links/connections
    for (const auto& pair : extract.getConnexions()) {
        const std::string& src = pair.first;
        for (const std::string& dest : pair.second) {
            addlink(src, dest);
        }
    }
}
