#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;





class Room {
private:
    string name;
    short Antmax;
    short Ant;
    vector<Room*> Direction;




public:
    // Constructor
    Room(const string& name, short Antmax);

    // Getters
    string getName() const;
    short getAntmax() const;
    short getAnt() const;
    const vector<Room*>& getDirection() const;

    // Methods
    void linkRoom(Room* neighborRoom);
    bool enter();
    void leave();
    bool isFull() const;
};



// Connections dictionaries
unordered_map<string, Room*> buildRoomGraph(
    const unordered_map<string, short>& dico1,
    const unordered_map<string, vector<string>>& dico2
);

#endif
