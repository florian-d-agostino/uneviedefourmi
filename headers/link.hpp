#ifndef LINK_HPP
#define LINK_HPP

#include <unordered_map>
#include <string>
#include "room.hpp"

class Extract; // Forward declaration

class Link {
private:
    std::unordered_map<std::string, Room*> rooms;
    Room* sv;
    Room* sd;

public:
    // Constructor
    Link();

    // Destructor (cleans up all dynamically allocated Room pointers)
    ~Link();

    // Methods
    void addroom(Room* room);
    void addlink(const std::string& nameS1, const std::string& nameS2);
    void buildFromExtract(const Extract& extract);

    // Getters
    Room* getSv() const;
    Room* getSd() const;
    const std::unordered_map<std::string, Room*>& getRooms() const;
};

#endif
